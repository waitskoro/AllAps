#pragma once

#include "common/messages.h"

#include <QObject>
#include <QTcpServer>

namespace Tcp {

class TcpSocket;

class TcpManager : public QObject
{
    Q_OBJECT
public:
    explicit TcpManager(QObject *parent = nullptr);

    void onMessageRecieved(Packet);
    void onServerCreating(const QString &ip, const int &port);

signals:
    void serverCreated();
    void clientConnected();
    void countMessage(Count);

private:
    QTcpServer *m_tcpServer;
    TcpSocket *m_tcpSocket;

    void onClientConnected();
};

}
