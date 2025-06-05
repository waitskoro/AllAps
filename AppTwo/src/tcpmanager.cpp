#include "tcpmanager.h"

#include "tcpsocket.h"

#include <QMessageBox>

using namespace Tcp;

TcpManager::TcpManager(QObject *parent)
    : QObject(parent)
    , m_tcpServer(new QTcpServer(this))
    , m_tcpSocket(new TcpSocket(this))
{
    connect(m_tcpServer,
            &QTcpServer::newConnection,
            this,
            &TcpManager::onClientConnected);

    connect(m_tcpSocket,
            &TcpSocket::messageRecieved,
            this,
            &TcpManager::onMessageRecieved);
}

void TcpManager::onMessageRecieved(Packet packet)
{
    QDataStream stream(&packet.data, QIODevice::ReadOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    if (packet.header.msgType == 0x82) {
        Count result;
        stream >> result;
        emit countMessage(result);
    }
}

void TcpManager::onServerCreating(const QString &ip, const int &port)
{
    QHostAddress hostAddress;

    if (ip.compare("localhost", Qt::CaseInsensitive) == 0) {
        hostAddress = QHostAddress::LocalHost;

    } else if (!hostAddress.setAddress(ip)) {
        qWarning() << "Invalid IP address: " << ip;
        return;
    }

    auto m_serverStarted = m_tcpServer->listen(hostAddress, port);
    if (!m_serverStarted) {
        qWarning() << "Server could not start" << m_tcpServer->errorString();

    } else {
        qInfo() << "Server started on" << ip << ":" << port;
        emit serverCreated();
    }
}

void TcpManager::onClientConnected()
{
    qInfo() << "Клиент подключен к серверу";
    m_tcpSocket = static_cast<TcpSocket*>(m_tcpServer->nextPendingConnection());
    emit clientConnected();
}
