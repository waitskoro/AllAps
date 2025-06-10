#include <QObject>
#include <QApplication>

#include "src/mainmanger.h"
#include "src/socket/tcpsocketmanager.h"
#include "src/connection/ipconnectionsetupmanager.h"

using namespace Connection;
using namespace Application;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TcpSocketManager *tcpManager = new TcpSocketManager();
    IpConnectionSetupManager *ipConnectionManager = new IpConnectionSetupManager();

    MainManager *mainManager = new MainManager(tcpManager);

    QObject::connect(ipConnectionManager,
                     &IpConnectionSetupManager::connectToHosts,
                     tcpManager,
                     &TcpSocketManager::connectToHost);

    QObject::connect(tcpManager,
                     &TcpSocketManager::connected,
                     [mainManager, ipConnectionManager](){
                         ipConnectionManager->close();
                         mainManager->showWindow();
                     });

    QObject::connect(mainManager,
                     &MainManager::reconnect,
                     [tcpManager, ipConnectionManager, mainManager](){
                         mainManager->closeWindow();

                         tcpManager->disconnect();
                         ipConnectionManager->open();
                     });

    return a.exec();
}
