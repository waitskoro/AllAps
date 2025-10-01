#include "ipconnectionsetupmanager.h"

#include "connectiondatasaving.h"
#include "gui/ipconnectionsetupwidget.h"

using namespace Connection;

IpConnectionSetupManager::IpConnectionSetupManager(QObject *parent)
    : QObject(parent)
    , m_connectionData(new ConnectionDataSaving())
    , m_connectionWidget(new View::IpConnectionSetupWidget())
{
    m_connectionWidget->showWindow(m_connectionData->connectionData());

    connect(m_connectionWidget,
            &View::IpConnectionSetupWidget::connectToHosts,
            this,
            [this](const QUrl &ac){
                m_connectionData->setConnectionData(ac);
                emit connectToHost(ac);
            });
}

void IpConnectionSetupManager::open()
{
    m_connectionWidget->showWindow(m_connectionData->connectionData());
}

void IpConnectionSetupManager::close()
{
    m_connectionWidget->close();
}

void IpConnectionSetupManager::connectingToHost()
{
    emit connectToHost(m_connectionData->connectionData().url);
}
