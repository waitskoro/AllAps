#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "src/tcpmanager.h"
#include "mainwindowmanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_tcpManager(new TcpManager(this))
    , m_mainWindowManager(new MainWindowManager(this))
{
    ui->setupUi(this);

    m_mainWindowManager->setUi(ui);
    m_mainWindowManager->setWidget(parent);

    connect(m_tcpManager,
            &TcpManager::serverCreated,
            m_mainWindowManager,
            &MainWindowManager::onServerCreated);

    connect(m_tcpManager,
            &TcpManager::clientConnected,
            m_mainWindowManager,
            &MainWindowManager::onClientConnected);

    connect(m_tcpManager,
            &TcpManager::countMessage,
            m_mainWindowManager,
            &MainWindowManager::onCountMessageRecieved);

    connect(m_mainWindowManager,
            &MainWindowManager::createServer,
            m_tcpManager,
            &TcpManager::onServerCreating);
}

MainWindow::~MainWindow()
{
    delete ui;
}

