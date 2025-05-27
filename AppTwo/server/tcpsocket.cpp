#include "tcpsocket.h"

#include "sequentialidprovider.h"

using namespace Tcp;

namespace {

const qint32 HEADER_SIZE = 16;

}

TcpSocket::TcpSocket(QObject *parent)
    : QTcpSocket(parent)
{
    connect(this,
            &QTcpSocket::connected,
            this,
            &TcpSocket::connected);

    connect(this,
            &QTcpSocket::readyRead,
            this,
            &TcpSocket::onReadyRead);

    connect(this,
            &QTcpSocket::disconnected,
            this,
            &TcpSocket::disconnected);
}

void TcpSocket::onReadyRead()
{
    if (!m_headerReaded) {
        if (bytesAvailable() >= HEADER_SIZE) {
            m_headerReaded = true;
            m_dataSize = m_header.countBytes;
            m_headerBytes = read(HEADER_SIZE);
            m_header = deserializeHeader(m_headerBytes);
        }
    }
    if (m_headerReaded) {
        if (m_dataSize > 0) {
            QByteArray chunk = read(qMin(m_dataSize,
                                         bytesAvailable()));
            m_msgBytes += chunk;;
            m_dataSize -= chunk.size();
        }
        if (m_dataSize == 0) {
            auto &provider = SequentialIdProvider::get();
            long long id = provider.next();

            Packet packet(m_header, m_msgBytes, id);

            emit messageReceived(packet);

            m_msgBytes.clear();
            m_headerBytes.clear();
            m_headerReaded = false;
        }
    }
}

Header TcpSocket::deserializeHeader(QByteArray &data)
{
    Header header;
    QDataStream stream(data);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream >> header.version
           >> header.msgType
           >> header.zero
           >> header.timeCreated
           >> header.countBytes;

    return header;
}
