#include "mainwindow.h"

#include <QPainter>

#include "infowidget.h"
#include "serverconnectingwidget.h"

using namespace View;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_label(new QLabel(this))
    , m_infoViewer(new InfoWidget(this))
    , m_serverConnecting(new ServerConnectingWidget(this))
{
    init();

    connect(
        m_serverConnecting,
        &ServerConnectingWidget::createServer,
        this,
        &MainWindow::createServer
    );
}

MainWindow::~MainWindow()
{}

void MainWindow::onServerCreated()
{
    m_label->hide();
    m_serverConnecting->hide();

    m_infoViewer->show();
}

void MainWindow::onClientConnected()
{

}

void MainWindow::onCountMessageRecieved(Report msg)
{
    m_infoViewer->addItem(msg);
}

void MainWindow::init()
{
    setFixedSize(700, 500);

    m_serverConnecting->show();
    m_serverConnecting->move(120, 10);

    m_label->setFixedSize(rect().size());
    m_label->setStyleSheet("color: white;"
                           "font-weight: 900;"
                           "font-size: 36px;");
    m_label->setText("Подключитесь к серверу для \nдоступа к функционалу");
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setWordWrap(true);

    m_infoViewer->setFixedSize(670, 440);
    m_infoViewer->move(15, 40);
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPixmap background(":/src/background.png");

    QPalette palette;

    palette.setBrush(QPalette::Window, background);
    this->setPalette(palette);

    QWidget::paintEvent(e);
}
