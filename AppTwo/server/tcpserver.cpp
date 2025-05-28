#include "tcpserver.h"

#define HEADER_SIZE 16

#include "sequentialidprovider.h"

TcpServer::TcpServer(QObject* parent) :
    QObject{parent}
{
    m_server = new QTcpServer(this);
    m_serverStarted = m_server->listen(QHostAddress::Any, 5555);
    if (!m_serverStarted) {
        qWarning() << "Server could not start";
    } else {
        qInfo() <<"Server started";
    }

    connect(m_server,
            &QTcpServer::newConnection,
            this,
            &TcpServer::onClientConnecting);
}

bool TcpServer::isStarted() const
{
    return m_serverStarted;
}

void TcpServer::onClientConnecting()
{
    qInfo() << "Клиент подключен к серверу";
    m_socket = m_server->nextPendingConnection();

    connect(m_socket,
            &QTcpSocket::readyRead,
            this,
            &TcpServer::onClientDataReady);

    connect(m_socket,
            &QTcpSocket::disconnected,
            this,
            &TcpServer::onClientDisconnected);

    emit connected();
}

void TcpServer::onClientDisconnected()
{
    m_serverStarted = false;
    qInfo() << "Клиент отключен";
}

void TcpServer::onClientDataReady()
{

    if(!header_readed) {
        if(m_socket->bytesAvailable() >= HEADER_SIZE){
            header_bytes = m_socket->read(HEADER_SIZE);
            header_ = deserializeHeader(header_bytes);
            data_size = header_.countBytes;
            header_readed = true;
        }
        else {
            return;
        }
    }
    if(header_readed) {
        if(data_size > 0) {
            QByteArray chunk = m_socket->read(qMin(data_size,m_socket->bytesAvailable()));
            msg_bytes += chunk;
            data_size -= chunk.size();
        }
        if(data_size <= 0){
            onMsgReceived(header_, msg_bytes);
            header_readed = false;
            msg_bytes.clear();
            header_bytes.clear();
        }
    }
}

void TcpServer::onMsgReceived(const Header& header, const QByteArray& msgData)
{
    auto &provider = SequentialIdProvider::get();
    long long id = provider.next();
    Packet packet(header, msgData, id);

    QDataStream stream(&packet.data, QIODevice::ReadOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
}

Header TcpServer::deserializeHeader(QByteArray &data)
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
