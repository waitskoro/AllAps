#include "tcpsocket.h"

#include "sequentialidprovider.h"

using namespace Application;

TcpSocket::TcpSocket(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket())
    , m_connectionTimeoutTimer(new QTimer(this))
{
    m_connectionTimeoutTimer->setInterval(2000);

    connect(m_connectionTimeoutTimer,
            &QTimer::timeout,
            this,
            &TcpSocket::onConnectionTimeout);

    connect(m_socket,
            &QTcpSocket::readyRead,
            this,
            &TcpSocket::handleReadyRead);

    connect(m_socket, &QTcpSocket::connected, this, &TcpSocket::onConnected);
}

void TcpSocket::handleReadyRead()
{
    if(!header_readed) {
        if(m_socket->bytesAvailable() >= m_headerSize){
            header_bytes = m_socket->read(m_headerSize);
            header_ = deserializeHeader(header_bytes);
            data_size = header_.countBytes;
            header_readed = true;
        }
        else {
            return;
        }
    }
    if(header_readed) {
        if(data_size >0) {
            QByteArray chunk = m_socket->read(qMin(data_size,m_socket->bytesAvailable()));
            msg_bytes += chunk;
            data_size -= chunk.size();
        }
        if(data_size == 0){
            auto& provider = SequentialIdProvider::get();
            long long id = provider.next();
            Packet packet(header_, msg_bytes,id);
            emit msgReceived(packet);
            header_readed = false;
            msg_bytes.clear();
            header_bytes.clear();
        }
    }
}

void TcpSocket::abortSocket()
{
    m_socket->abort();
}

void TcpSocket::disconnectFromHost()
{
    m_connectionTimeoutTimer->stop();
    m_socket->close();
}

bool TcpSocket::isConnected() const
{
    return m_socket->state() == QAbstractSocket::SocketState::ConnectedState;
}

void TcpSocket::connectToHost(const QUrl& url)
{
    if (m_socket->isOpen())
        m_socket->close();

    m_url = url;
    m_socket->connectToHost(m_url.host(), m_url.port());

    m_connectionTimeoutTimer->start();
}

void TcpSocket::send(const QByteArray& header, const QByteArray& msg)
{
    if (isConnected()) {
        m_socket->write(header);
        m_socket->write(msg);
    }
}

QAbstractSocket::SocketState TcpSocket::state() const
{
    return m_socket->state();
}

void TcpSocket::onConnectionTimeout()
{
    m_socket->connectToHost(m_url.host(), m_url.port());

    if (m_socket->state() == QAbstractSocket::ConnectingState) {
        emit connectionTakingTooLong();
    }
}

void TcpSocket::onConnected()
{
    m_connectionTimeoutTimer->stop();
    emit connected();
}

Header TcpSocket::deserializeHeader(const QByteArray& data) const
{
    Header header;
    QDataStream stream(data);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream >> header.version
        >> header.msg_type
        >> header.zero
        >> header.timeCreated
        >> header.countBytes;

    return header;
}
