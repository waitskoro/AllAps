#pragma once

#include "common/messages.h"

#include <QObject>
#include <QTcpSocket>

namespace Tcp {

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent = nullptr);

signals:
    void messageRecieved(Packet &);

private:
    void onReadyRead();

    Header m_header;
    qint64 m_dataSize;
    QByteArray m_msgBytes;
    QByteArray  m_headerBytes;

    bool m_headerReaded;

    Header deserializeHeader(QByteArray& data);
};

}
