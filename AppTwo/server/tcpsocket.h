#pragma once

#include "packet.h"

#include <QTcpSocket>

namespace Tcp {

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent = nullptr);

signals:
    void connected();
    void disconnected();

    void messageReceived(Packet &);

private:
    void onReadyRead();

    Header m_header;
    qint64 m_dataSize;
    QByteArray m_msgBytes;
    QByteArray  m_headerBytes;

    bool m_headerReaded;

    void onChannelDataReceived(Packet &);
    Header deserializeHeader(QByteArray& data);
};

}
