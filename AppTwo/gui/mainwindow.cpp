#include "mainwindow.h"

#include <QTimer>
#include <QThread>
#include <QPainter>
#include <QDateTime>

#include "infowidget.h"
#include "graph/graphwidget.h"
#include "serverconnectingwidget.h"

using namespace View;
using namespace Graph;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_label(new QLabel(this))
    , m_timer(new QTimer(this))
    , m_dataTimeoutTimer(new QTimer(this))
    , m_tabWidget(new QTabWidget(this))
    , m_infoViewer(new InfoWidget(this))
    , m_graphViewer(new GraphWidget())
    , m_serverConnecting(new ServerConnectingWidget(this))
{
    init();

    m_tabWidget->hide();

    connect(m_serverConnecting,
            &ServerConnectingWidget::createServer,
            this,
            &MainWindow::createServer);

    m_timer->setInterval(1000);

    m_dataTimeoutTimer->setInterval(500);
    m_dataTimeoutTimer->setSingleShot(true);
    connect(m_dataTimeoutTimer, &QTimer::timeout, [this]() {
        if (m_timer->isActive()) {
            m_timer->stop();
            qDebug() << "Данные не поступают, таймер остановлен";
        }
    });
}

MainWindow::~MainWindow()
{
    delete m_label;
    delete m_tabWidget;
    delete m_infoViewer;
    delete m_graphViewer;
    delete m_serverConnecting;
}

void MainWindow::onServerCreated()
{
    m_label->hide();
    m_serverConnecting->hide();

    m_tabWidget->show();
}

void MainWindow::onClientConnected()
{
    qDebug() << "Клиент подключился";
}

void MainWindow::onCountMessageRecieved(const Report &msg)
{
    m_newDataAvailable = true;
    m_lastDateTime = msg.time;

    m_infoViewer->addItem(msg);

    if (!m_graphViewer->channelMatch(msg.channel))
        return;

    m_dataTimeoutTimer->start();
    if (!m_timer->isActive()) {
        m_timer->start();
        qDebug() << "Поступили новые данные, таймер запущен";
    }

    m_graphViewer->onSamplesReaded(convertToComplex(msg.info));
}

void MainWindow::init()
{
    setFixedSize(700, 800);

    m_serverConnecting->show();
    m_serverConnecting->move(120, 10);

    m_label->setFixedSize(rect().size());
    m_label->setStyleSheet("color: white;"
                           "font-weight: 900;"
                           "font-size: 36px;");
    m_label->setText("Подключитесь к серверу для \nдоступа к функционалу");
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setWordWrap(true);

    m_tabWidget->move(15, 50);
    m_tabWidget->setFixedSize(670, 700);
    m_tabWidget->addTab(m_infoViewer, "Отсчеты");
    m_tabWidget->addTab(m_graphViewer, "Графики");

    m_tabWidget->setStyleSheet("color: rgb(119, 133, 255);"
                               "font-weight: 600;");
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPixmap background(":/src/background.png");

    background = background.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(0, 0, background);

    QWidget::paintEvent(e);
}

ComplexVector MainWindow::convertToComplex(const QVector<std::array<qint8, 2>> &data)
{
    ComplexVector result;

    for (const auto& item : data)
        result.append(Complex(item[0], item[1]));

    return result;
}
