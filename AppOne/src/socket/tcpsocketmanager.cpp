#include "tcpsocketmanager.h"

#include "tcpsocket.h"
#include "src/contants.h"
#include "src/messagesinfo.h"


using namespace Application;

TcpSocketManager::TcpSocketManager(QObject *parent)
    : QObject(parent)
    , m_socket(new TcpSocket())
    , m_logFile("../log.txt")
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

    qInfo() << "Подключение к сокету...";
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

    qInfo() << "Команда на передачу целеуказаний отправлена";
}

void TcpSocketManager::stopingPlans()
{
    sendEmptyPacket(0x02);
    qInfo() << "Команда остановки планов отправлена";
}

void TcpSocketManager::sendRequestStateWorking()
{
    sendEmptyPacket(0x03);
    qInfo() << "Запрос состояния работоспособности АС отправлен";
}

void TcpSocketManager::sendRequestTraskingPlans()
{
    sendEmptyPacket(0x04);
    qInfo() << "Запрос о состоянии каналов данных отправлен";
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
        qInfo() << "Успешное подключение к сокету";
        isSocketsConnected = true;
        emit connected();
    }
    else {
        qWarning() << "Ошибка подключения к сокету";
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

        if (result.result == 0) {
            qInfo() << "Получен результат исполнения команды: Успешно";
        } else {
            qWarning() << "Получен результат исполнения команды. Код ошибки:" << result.result;
        }

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
        qInfo() << "Результат исполнения команды о текущем состоянии АС получен";
    }
    if (packet.header.msg_type == 0x84) {
        DataChannelMessage dataChannelMsg;
        stream >> dataChannelMsg;
        emit recievedTrackingPlans(dataChannelMsg);
        qInfo() << "Результат исполнения команды состояния каналов данных получен";
    }
}
