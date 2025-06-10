#include "tcpsocketmanager.h"

#include "tcpsocket.h"
#include "src/contants.h"
#include "src/messagesinfo.h"

using namespace Application;

TcpSocketManager::TcpSocketManager(QObject *parent)
    : QObject(parent)
    , m_socket(new TcpSocket())
{
    connect(m_socket,
            &TcpSocket::connected,
            this,
            &TcpSocketManager::onConnected);

    connect(m_socket,
            &TcpSocket::msgReceived,
            this,
            &TcpSocketManager::onSocketAcPacketReceived);
}

void TcpSocketManager::disconnect()
{
    isSocketsConnected = false;
    m_socket->disconnectFromHost();
    emit stateChanged(Disconnected);

    qInfo() << "Отключение от сокета";
}

void TcpSocketManager::cancelConnecting()
{
    isSocketsConnected = false;
}

void TcpSocketManager::connectToHost(const QUrl &ac)
{
    m_url = ac;
    m_socket->connectToHost(ac);
    emit stateChanged(Connecting);

    qInfo() << "Подключение к сокету";
}

void TcpSocketManager::sendTarget(TargetDesignations target)
{
    Header header;
    header.msg_type = 0x01;
    header.countBytes = 26 + 4 * target.count;
    header.timeCreated = currentOADate();

    QByteArray header_bytes;
    QByteArray message_bytes;

    header_bytes = header.serializeStruct();
    QDataStream stream(&message_bytes, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream << target;

    m_socket->send(header_bytes, message_bytes);
}

void TcpSocketManager::stopingPlans()
{
    sendEmptyPacket(0x02);
}

void TcpSocketManager::sendRequestStateWorking()
{
    sendEmptyPacket(0x03);
}

void TcpSocketManager::sendRequestTraskingPlans()
{
    sendEmptyPacket(0x04);
}

void TcpSocketManager::sendEmptyPacket(uint8_t type)
{
    Header header;
    header.msg_type = type;
    header.countBytes = 0;
    header.timeCreated = currentOADate();

    QByteArray header_bytes;
    QByteArray message_bytes;

    header_bytes = header.serializeStruct();
    m_socket->send(header_bytes, message_bytes);
}

void TcpSocketManager::onConnected()
{
    if (m_socket->isConnected() && !isSocketsConnected) {
        isSocketsConnected = true;
        emit connected();
    }
}

void TcpSocketManager::onSocketAcPacketReceived(Packet packet)
{
    QDataStream stream(&packet.data, QIODevice::ReadOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    if (packet.header.msg_type == 0x80) {
        ExecutedTheCommand result;
        stream >> result;
        emit executedTheCommandRecevied(result);
    }
    if (packet.header.msg_type == 0x81) {
        RecieveState receiveState;
        stream >> receiveState;

        for (quint8 i = 0; i < receiveState.n; ++i) {
            const auto& ch = receiveState.chanel_mas[i];
            emit receivingMessage(ch);
        }

        if (receiveState.n == 0) {
            emit receivingMessageEmpty();
        }
    }
    if (packet.header.msg_type == 0x83) {
        StateMessage result;
        stream >> result;
        emit recieveState(result);
    }
    if (packet.header.msg_type == 0x84) {
        DataChannelMessage dataChannelMsg;
        stream >> dataChannelMsg;
        emit recievedTrackingPlans(dataChannelMsg);
    }
}
