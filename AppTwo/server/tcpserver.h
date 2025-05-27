#pragma once

#include <QUrl>
#include <QTimer>
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>

#include "packet.h"

namespace Tcp {

class TcpSocket;

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);

    void disconnect();
    void connectToServer(const QString &ip, const QString &port);
signals:
    void connected();
    void connecting();
    void disconnected();

    void messageReceived(const Packet &);

private slots:
    void onClientConnected();

private:
    Header m_header;
    qint64 m_dataSize;
    QByteArray m_msgBytes;
    QByteArray  m_headerBytes;

    bool m_headerReaded;
    bool m_isServerStarted;

    TcpSocket *m_socket;
    QTcpServer *m_server;
};

}
