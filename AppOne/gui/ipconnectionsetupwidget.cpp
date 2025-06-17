#include "ipconnectionsetupwidget.h"

#include <QLabel>
#include <QThread>
#include <QPainter>

#include "gui/loader.h"
#include "src/contants.h"
#include "src/connection/connectiondatasaving.h"

using namespace View;

IpConnectionSetupWidget::IpConnectionSetupWidget(QWidget *parent)
    : QWidget(parent)
    , m_mainRect(QRect(0, 0, 400, 150))
{
    initUI();
    m_loader->stop();
}

void IpConnectionSetupWidget::showWindow(Connection::ConnectionData data)
{
    m_acIpTextEdit->setText(data.acHost);
    m_acPortTextEdit->setText(data.acPort);

    m_loader->stop();

    show();
}

void IpConnectionSetupWidget::initUI()
{
    setFixedSize(Sizes::authSize());
    setStyleSheet(Styles::mainColor());

    m_acIpTextEdit = createLineEdit(180, 30);
    m_acPortTextEdit = createLineEdit(80, 30);
    m_pushButton = createButton(this, "Соединиться", 150, 35);

    m_loader = new Loader("Подключение", QRect(0, 0, 650, 400), this);

    connect(m_pushButton,
            &QPushButton::clicked,
            this,
            &IpConnectionSetupWidget::onPushButtonClicked);

    updateWidgetPositions();
}

QLineEdit* IpConnectionSetupWidget::createLineEdit(int width, int height)
{
    auto edit = new QLineEdit(this);
    edit->setFixedSize(width, height);
    edit->setStyleSheet("background-color: white");
    return edit;
}

void IpConnectionSetupWidget::updateWidgetPositions()
{
    m_mainRect.moveTo(centeringCoordinates(Sizes::authSize(), m_mainRect.size()));

    m_acIpTextEdit->move(m_mainRect.x() + 55, m_mainRect.y() + 60);
    m_acPortTextEdit->move(m_mainRect.x() + 305, m_mainRect.y() + 60);

    m_pushButton->move(
        m_mainRect.x() + centerX(m_mainRect.size(), m_pushButton->size()),
        m_mainRect.y() + 100
    );
}

void IpConnectionSetupWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPainter painter(this);
    painter.fillRect(m_mainRect, "#C3D7E4");
    painter.drawRect(m_mainRect);

    drawLabels(painter);
}

void IpConnectionSetupWidget::drawLabels(QPainter &painter)
{
    painter.setClipRect(m_mainRect);

    QFont font;
    font.setBold(true);
    font.setWeight(QFont::Medium);
    font.setPixelSize(20);
    painter.setFont(font);

    painter.drawText(m_mainRect.x() + 30, m_mainRect.y() + 40, "Подключение к сокету");

    font.setWeight(QFont::Light);
    painter.setFont(font);

    painter.drawText(m_mainRect.x() + 30, m_mainRect.y() + 80, "IP");
    painter.drawText(m_mainRect.x() + 250, m_mainRect.y() + 80, "Порт");
}

void IpConnectionSetupWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    updateWidgetPositions();
}

void IpConnectionSetupWidget::onPushButtonClicked()
{
    auto acIp = m_acIpTextEdit->text();
    auto acPort = m_acPortTextEdit->text();

    if (acIp != "" && acPort != "") {
        QUrl url_ac = QUrl::fromUserInput(acIp + ":" + acPort);
        emit connectToHosts(url_ac);
    }

    m_loader->start();
    m_loader->setAttribute(Qt::WA_AlwaysStackOnTop);
}

IpConnectionSetupWidget::~IpConnectionSetupWidget()
{
    deleteLater();
}
