#include "tcpsocket.h"
#include "sequentialidprovider.h"

using namespace Application;

TcpSocket::TcpSocket(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket())
    , m_connectionTimeoutTimer(new QTimer(this))
    , m_reconnectTimer(new QTimer(this))
{
    m_connectionTimeoutTimer->setInterval(2000);
    m_connectionTimeoutTimer->setSingleShot(true);

    m_reconnectTimer->setInterval(2000);
    m_reconnectTimer->setSingleShot(true);

    connect(m_connectionTimeoutTimer, &QTimer::timeout, this, &TcpSocket::onConnectionTimeout);
    connect(m_reconnectTimer, &QTimer::timeout, this, &TcpSocket::safeConnectToHost);

    connect(m_socket, &QTcpSocket::readyRead, this, &TcpSocket::handleReadyRead);
    connect(m_socket, &QTcpSocket::connected, this, &TcpSocket::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, [this]() {
        resetReadState();
        if (m_autoReconnect) {
            scheduleReconnect();
        }
    });
    connect(m_socket, &QTcpSocket::stateChanged, this, &TcpSocket::onSocketStateChanged);
    connect(m_socket, &QAbstractSocket::errorOccurred, this, &TcpSocket::onSocketError);
}

TcpSocket::~TcpSocket()
{
    disconnectFromHost();
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
        if(data_size > 0) {
            QByteArray chunk = m_socket->read(qMin(data_size, m_socket->bytesAvailable()));
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

void TcpSocket::disconnectFromHost()
{
    m_connectionTimeoutTimer->stop();
    m_reconnectTimer->stop();
    m_autoReconnect = false;

    if (m_socket->state() != QAbstractSocket::UnconnectedState) {
        m_socket->disconnectFromHost();
        if (m_socket->state() == QAbstractSocket::ConnectedState) {
            m_socket->waitForDisconnected(1000);
        }
    }
    resetReadState();
}

bool TcpSocket::isConnected() const
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

void TcpSocket::connectToHost(const QUrl& url)
{
    m_connectionTimeoutTimer->stop();
    m_reconnectTimer->stop();
    m_autoReconnect = true;

    if (m_socket->state() != QAbstractSocket::UnconnectedState) {
        m_socket->abort();
    }

    resetReadState();
    m_url = url;

    safeConnectToHost();
}

void TcpSocket::safeConnectToHost()
{
    if (m_socket->state() == QAbstractSocket::UnconnectedState) {
        qDebug() << "Попытка подключения к" << m_url.toString();
        m_socket->connectToHost(m_url.host(), m_url.port());
        m_connectionTimeoutTimer->start();
    } else {
        qWarning() << "Попытка подключения при активном состоянии:" << m_socket->state();
        QTimer::singleShot(500, [this]() {
            if (m_socket->state() != QAbstractSocket::UnconnectedState) {
                m_socket->abort();
            }
            QTimer::singleShot(100, this, &TcpSocket::safeConnectToHost);
        });
    }
}

void TcpSocket::scheduleReconnect()
{
    if (m_autoReconnect && !m_reconnectTimer->isActive()) {
        qDebug() << "Запланировано автоматическое переподключение через" << m_reconnectTimer->interval() << "мс";
        m_reconnectTimer->start();
    }
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
    if (m_socket->state() == QAbstractSocket::ConnectingState ||
        m_socket->state() == QAbstractSocket::HostLookupState) {
        qWarning() << "Таймаут подключения, повторная попытка...";
        m_socket->abort();
        safeConnectToHost();
    }
}

void TcpSocket::onConnected()
{
    m_connectionTimeoutTimer->stop();
    m_reconnectTimer->stop();
    resetReadState();
    qDebug() << "Успешное подключение к сокету";
    emit connected();
}

void TcpSocket::onSocketStateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << "Состояние сокета изменилось на:" << state;

    switch (state) {
    case QAbstractSocket::UnconnectedState:
        resetReadState();
        m_connectionTimeoutTimer->stop();
        break;
    case QAbstractSocket::ConnectedState:
        m_connectionTimeoutTimer->stop();
        m_reconnectTimer->stop();
        break;
    default:
        break;
    }
}

void TcpSocket::onSocketError(QAbstractSocket::SocketError error)
{
    QString errorString = "Ошибка соединения: " + m_socket->errorString();
    qWarning() << errorString;

    m_connectionTimeoutTimer->stop();
    resetReadState();

    switch (error) {
    case QAbstractSocket::RemoteHostClosedError:
        if (m_autoReconnect) {
            scheduleReconnect();
        }
        emit remoteHostClosed();
        break;
    case QAbstractSocket::ConnectionRefusedError:
    case QAbstractSocket::HostNotFoundError:
    case QAbstractSocket::SocketTimeoutError:
        if (m_autoReconnect) {
            scheduleReconnect();
        }
        emit errorOccurred(errorString);
        break;
    default:
        emit errorOccurred(errorString);
        break;
    }
}

void TcpSocket::resetReadState()
{
    header_readed = false;
    header_bytes.clear();
    msg_bytes.clear();
    data_size = 0;
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
