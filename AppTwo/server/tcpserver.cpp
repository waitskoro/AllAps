#include "tcpserver.h"

#include "tcpsocket.h"

using namespace Tcp;

TcpServer::TcpServer(QObject *parent)
    : QObject(parent)
    , m_socket(nullptr)
    , m_server(new QTcpServer(this))
{
    connect(m_server,
            &QTcpServer::newConnection,
            this,
            &TcpServer::onClientConnected);
}

void TcpServer::disconnect()
{
    m_server->disconnect();

    if (m_socket) {
        m_socket->disconnect();
        m_socket->close();
    }

    m_socket = nullptr;
}

void TcpServer::connectToServer(const QString &ip, const QString &port)
{
    if (ip.isEmpty() || port.isEmpty()) {
        qInfo() << "Запуск сервера с данными по умолчанию: localhost:5555";
        m_isServerStarted = m_server->listen(QHostAddress::Any, 5555);
    } else {
        m_isServerStarted = m_server->listen(QHostAddress(ip), port.toUShort());
        qInfo() << QString("Запуск сервера с данными по умолчанию: %1:%2").arg(ip).arg(port);
    }

    if (!m_isServerStarted) {
        qWarning() << "Сервер невозможно запустить";
    } else {
        qInfo() <<"Сервер запущен";
        emit connected();
    }
}

void TcpServer::onClientConnected()
{
    qInfo() << "Клиент подключен к серверу";

    QTcpSocket *rawSocket = m_server->nextPendingConnection();

    if (rawSocket) {
        m_socket = new TcpSocket(this);
        m_socket->setSocketDescriptor(rawSocket->socketDescriptor());
        rawSocket->deleteLater();
    }

    connect(m_socket,
            &TcpSocket::messageReceived,
            this,
            &TcpServer::messageReceived);
}
