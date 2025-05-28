#include "mainwindow.h"

#include "connectionheader.h"
#include "server/tcpserver.h"

#include "graphs/graphsviewwidget.h"
#include "info/informationviewwidget.h"

#include <QStyle>
#include <QPainter>
#include <QVBoxLayout>

using namespace View;

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_tabWidget(new QTabWidget(this))
    , m_connectionHeader(new ConnectionHeader(this))
    , m_menu(new QPushButton(this))
    , m_server(new TcpServer(this))
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

    m_tabWidget->move(0, 40);
    m_tabWidget->setFixedSize(700, 450);

    InformationViewWidget *info = new InformationViewWidget(this);
    m_tabWidget->addTab(info, "Информация об отсчетах");

    GraphsViewWidget *graph = new GraphsViewWidget(this);
    m_tabWidget->addTab(graph, "Графики");

    m_menu->setStyleSheet(QString("background-color: #3C388D"));

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

    // connect(m_connectionHeader,
    //         &ConnectionHeader::connectTo,
    //         m_server,
    //         &TcpServer::connectToServer);

    // connect(m_connectionHeader,
    //         &ConnectionHeader::disconnect,
    //         m_server,
    //         &TcpServer::disconnect);

    connect(m_server,
            &TcpServer::connected,
            [this](){
                m_connectionHeader->setVisible(false);
                m_connectionHeader->onConnected();
            });

    // connect(m_server,
    //         &TcpServer::channelDataReceived,
    //         info,
    //         &InformationViewWidget::addChannelData);
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter;

    painter.begin(this);

    painter.fillRect(rect(), QColor(57, 57, 57, 30));

    painter.end();

    QWidget::paintEvent(e);
}

MainWindow::~MainWindow() {}
