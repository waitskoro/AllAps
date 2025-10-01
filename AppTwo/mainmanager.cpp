#include "mainmanager.h"

#include "src/tcpmanager.h"
#include "gui/mainwindow.h"

#include <QThread>

MainManager::MainManager(QObject *parent)
    : QObject(parent)
    , m_mainView(new View::MainWindow())
    , m_tcpManager(new Tcp::TcpManager(this))
{
    m_mainView->show();

    connect(m_mainView,
            &View::MainWindow::createServer,
            m_tcpManager,
            &Tcp::TcpManager::onServerCreating);

    connect(m_tcpManager,
            &Tcp::TcpManager::serverCreated,
            m_mainView,
            &View::MainWindow::onServerCreated);

    connect(m_tcpManager,
            &Tcp::TcpManager::clientConnected,
            m_mainView,
            &View::MainWindow::onClientConnected);

    connect(m_tcpManager,
            &Tcp::TcpManager::countMessage,
            m_mainView,
            &View::MainWindow::onCountMessageRecieved);


}

MainManager::~MainManager()
{}
