#include "mainmanger.h"

#include "gui/mainwidget.h"
#include "src/socket/tcpsocketmanager.h"

using namespace Application;

MainManager::MainManager(TcpSocketManager *tcpManager, QObject *parent)
    : QObject(parent)
    , m_tcpManager(tcpManager)
    , m_mainWidget(new View::MainWidget())
{
    //Commands
    connect(m_mainWidget,
            &View::MainWidget::sendTarget,
            m_tcpManager,
            &TcpSocketManager::sendTarget);

    connect(m_mainWidget,
            &View::MainWidget::sendStopingPlans,
            m_tcpManager,
            &TcpSocketManager::stopingPlans);

    connect(m_tcpManager,
            &TcpSocketManager::receivingMessage,
            m_mainWidget,
            &View::MainWidget::addMessage);

    connect(m_tcpManager,
            &TcpSocketManager::executedTheCommandRecevied,
            m_mainWidget,
            &View::MainWidget::executedTheCommandRecevied);

    //Requests
    connect(m_mainWidget,
            &View::MainWidget::sendRequestTrackingPlans,
            m_tcpManager,
            &TcpSocketManager::sendRequestTraskingPlans);

    connect(m_tcpManager,
            &TcpSocketManager::recievedTrackingPlans,
            m_mainWidget,
            &View::MainWidget::dataChannelMessagesRecieved);

    connect(m_mainWidget,
            &View::MainWidget::sendRequestState,
            m_tcpManager,
            &TcpSocketManager::sendRequestStateWorking);

    connect(m_tcpManager,
            &TcpSocketManager::recieveState,
            m_mainWidget,
            &View::MainWidget::recieveState);

    //Tcp
    connect(m_mainWidget,
            &View::MainWidget::disconnect,
            this,
            &MainManager::disconnect);
}

void MainManager::showWindow()
{
    m_mainWidget->showWindow();
}

void MainManager::closeWindow()
{
    m_mainWidget->close();
}
