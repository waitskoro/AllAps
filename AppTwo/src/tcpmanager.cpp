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

    connect(m_tcpSocket, &QTcpSocket::stateChanged, [](int state) {

        if (state == QAbstractSocket::UnconnectedState)
        {
            qDebug() << "TcpSocket disconected";
        }
        else if (state == QAbstractSocket::ConnectedState)
        {
            qDebug() << "TcpSocket connected";
        }
    });
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
        qWarning() << "Невалидный IP: " << ip;
        return;
    }

    auto m_serverStarted = m_tcpServer->listen(hostAddress, port);
    if (!m_serverStarted) {
        qWarning() << "Сервер не может быть запущен" << m_tcpServer->errorString();

    } else {
        qInfo() << "Сервер запущен с" << ip << ":" << port;
        emit serverCreated();
    }
}

void TcpManager::onClientConnected()
{
    qInfo() << "Клиент подключен к серверу";
    m_tcpSocket = static_cast<TcpSocket*>(m_tcpServer->nextPendingConnection());
    emit clientConnected();

    connect(m_tcpSocket,
            &TcpSocket::disconnected,
            this,
            &TcpManager::onClientDisconnected);
}

void TcpManager::onClientDisconnected()
{
    qInfo() << "Клиент отключился от сервера";
}
