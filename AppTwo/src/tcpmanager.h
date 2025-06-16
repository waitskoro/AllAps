#pragma once

#include "common/messages.h"

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>

namespace Reports {
class CsvParser;
}

namespace Tcp {

class TcpSocket;

class TcpManager : public QObject
{
    Q_OBJECT
public:
    explicit TcpManager(QObject *parent = nullptr);

    void onMessageRecieved(Packet &);
    void onServerCreating(const int &port);

signals:
    void serverCreated();
    void clientConnected();
    void countMessage(const Report &result);

private:
    QTcpServer *m_tcpServer;
    QTcpSocket* m_tcpSocket;

    void onReadyRead();

    void onClientConnected();
    void onClientDisconnected();

    Header m_header;
    qint64 m_dataSize = 0;
    QByteArray m_msgBytes;
    QByteArray  m_headerBytes;

    bool m_headerReaded;

    Header deserializeHeader(QByteArray& data);

    Reports::CsvParser *m_csvParser;
};

}
