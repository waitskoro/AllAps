#include "tcpmanager.h"

#include "binparser.h"
#include "sequentialidprovider.h"


#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>

namespace {
const qint32 HEADER_SIZE = 16;
}

TcpManager::TcpManager(QObject *parent)
    : QObject(parent)
    , m_tcpServer(new QTcpServer(this))
    , m_binParser(new BinParser())
{
    connect(m_tcpServer,
            &QTcpServer::newConnection,
            this,
            &TcpManager::onClientConnected);

    connect(&m_ppsTimer, &QTimer::timeout, [this]() {
        qDebug() << "PACKETS PER SECOND:" << m_packetCounter;
        m_packetCounter = 0;
    });
}

void TcpManager::onMessageRecieved(const Packet &packet)
{
    QDataStream stream(packet.data);
    stream.setByteOrder(QDataStream::LittleEndian);

    if (packet.header.msgType == 0x82) {
        Report result;
        stream >> result;

        m_packetCounter++;

        emit countMessage(result);

        QVector<QPair<int, int>> data;
        data.reserve(result.count);
        for (quint32 i = 0; i < result.count; i++) {
            data.append({result.info[i][0], result.info[i][1]});
        }

        m_binParser->appendChannelDataBatch(result.channel, data);
    }
}

void TcpManager::onServerCreating(const int &port)
{
    auto started = m_tcpServer->listen(QHostAddress::Any, port);

    QHostAddress anyAddress = QHostAddress::Any;
    QString anyAddressString = anyAddress.toString();

    if (!started) {
        qWarning() << "Сервер не может быть запущен";
    } else {
        qInfo() << "Сервер запущен";
        emit serverCreated();
    }
}

void TcpManager::onClientConnected()
{
    qDebug() << "Клиент подключен";

    m_ppsTimer.start(1000);

    m_tcpSocket = m_tcpServer->nextPendingConnection();
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &TcpManager::onReadyRead);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, [this]() {
        m_tcpSocket->deleteLater();
    });

    emit clientConnected();
}

void TcpManager::onReadyRead()
{
    while (m_tcpSocket->bytesAvailable() >= HEADER_SIZE) {
        if (!m_headerReaded) {
            m_headerBytes = m_tcpSocket->read(HEADER_SIZE);
            m_header = deserializeHeader(m_headerBytes);
            m_dataSize = m_header.countBytes;
            m_headerReaded = true;
        }

        if (m_headerReaded && m_dataSize > 0) {
            QByteArray chunk = m_tcpSocket->read(m_dataSize);
            m_msgBytes += chunk;
            m_dataSize -= chunk.size();

            if (m_dataSize == 0) {
                auto &provider = SequentialIdProvider::get();
                Packet packet(m_header, m_msgBytes, provider.next());
                onMessageRecieved(packet);

                m_headerReaded = false;
                m_msgBytes.clear();
                m_headerBytes.clear();
            }
        }
    }
}

void TcpManager::resetState()
{
    m_headerReaded = false;
    m_msgBytes.clear();
    m_headerBytes.clear();
    m_dataSize = 0;
}

Header TcpManager::deserializeHeader(QByteArray& data)
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
void TcpManager::onClientDisconnected()
{
    m_ppsTimer.start(1000);
    qInfo() << "Клиент отключился от сервера";
}
