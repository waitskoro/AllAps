#include <QFile>
#include <QObject>
#include <QDateTime>
#include <QApplication>

#include "src/mainmanger.h"
#include "src/socket/tcpsocketmanager.h"
#include "src/connection/ipconnectionsetupmanager.h"

#include <iostream>
using namespace std;

using namespace Connection;
using namespace Application;

QScopedPointer<QFile> m_logFile;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    m_logFile.reset(new QFile("../logFile.txt"));
    m_logFile.data()->open(QFile::Append | QFile::Text);
    qInstallMessageHandler(messageHandler);

    TcpSocketManager *tcpManager = new TcpSocketManager();
    IpConnectionSetupManager *ipConnectionManager = new IpConnectionSetupManager();

    MainManager *mainManager = new MainManager(tcpManager);

    QObject::connect(ipConnectionManager,
                     &IpConnectionSetupManager::connectToHost,
                     tcpManager,
                     &TcpSocketManager::connectToHost);

    QObject::connect(tcpManager,
                     &TcpSocketManager::connected,
                     [mainManager, ipConnectionManager](){
                         ipConnectionManager->close();
                         mainManager->showWindow();
                     });

    QObject::connect(mainManager,
                     &MainManager::disconnect,
                     [tcpManager, ipConnectionManager, mainManager](){
                         mainManager->closeWindow();

                         tcpManager->disconnect();
                         ipConnectionManager->open();
                         ipConnectionManager->connectingToHost();
                     });

    return a.exec();
}

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context)

    QString logMessage = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");

    switch (type) {
        case QtInfoMsg:     logMessage += "[INFO] "; break;
        case QtDebugMsg:    logMessage += "[DEBUG] "; break;
        case QtWarningMsg:  logMessage += "[WARNING] "; break;
        case QtCriticalMsg: logMessage += "[CRITICAL] "; break;
        case QtFatalMsg:    logMessage += "[FATAL] "; break;
    }

    logMessage += ": ";
    logMessage += msg;

    QTextStream out(m_logFile.data());
    out << logMessage << Qt::endl;
    out.flush();

    cout << logMessage.toStdString() << endl;
}
