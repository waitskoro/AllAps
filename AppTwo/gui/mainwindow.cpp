#include "mainwindow.h"

#include "connectionheader.h"
#include "server/tcpserver.h"

#include <QStyle>
#include <QPainter>
#include <QVBoxLayout>

using namespace View;

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_connectionHeader(new ConnectionHeader(this))
    , m_menu(new QPushButton(this))
    , m_server(new Tcp::TcpServer(this))
{
    setFixedSize(700, 600);
    setStyleSheet("background-color: white");

    m_menu->move(-10, 0);
    m_menu->setFixedSize(720, 30);

    QPixmap pixmap = style()->standardIcon(QStyle::SP_MediaPlay).pixmap(16, 16);
    pixmap = pixmap.transformed(QTransform().rotate(m_iconRotationAngle),
                                Qt::SmoothTransformation);
    m_menu->setIcon(QIcon(pixmap));
    m_menu->setText(QString(" ").repeated(190));

    connect(m_menu,
            &QPushButton::clicked,
            [this](){
                auto visible = m_connectionHeader->isVisible();
                m_connectionHeader->setVisible(!visible);

                m_iconRotationAngle = (m_iconRotationAngle == 90) ? -90 : 90;

                QPixmap pixmap = style()->standardIcon(QStyle::SP_MediaPlay).pixmap(16, 16);
                pixmap = pixmap.transformed(QTransform().rotate(m_iconRotationAngle),
                                            Qt::SmoothTransformation);
                m_menu->setIcon(QIcon(pixmap));
            });

    connect(m_connectionHeader,
            &ConnectionHeader::connectTo,
            this,
            &MainWindow::onConnectTo);

    connect(m_connectionHeader,
            &ConnectionHeader::disconnect,
            m_server,
            &Tcp::TcpServer::disconnect);

    connect(m_server,
            &Tcp::TcpServer::connected,
            this,
            [this](){
                m_connectionHeader->setVisible(false);
                m_connectionHeader->onConnected();
            });
}

void MainWindow::onConnectTo(const QString &ip, const QString &port)
{
    m_server->connectToServer(ip, port);
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter;

    painter.begin(this);

    painter.end();

    QWidget::paintEvent(e);
}

MainWindow::~MainWindow() {}
