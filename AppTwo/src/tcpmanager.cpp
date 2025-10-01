#include "tcpmanager.h"

#include "src/csvparser.h"
#include "sequentialidprovider.h"

#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>

using namespace Tcp;

namespace {
const qint32 HEADER_SIZE = 16;
}

TcpManager::TcpManager(QObject *parent)
    : QObject(parent)
    , m_tcpServer(new QTcpServer(this))
    , m_csvParser(new Reports::CsvParser())
{
    connect(m_tcpServer,
            &QTcpServer::newConnection,
            this,
            &TcpManager::onClientConnected);

    connect(&m_ppsTimer, &QTimer::timeout, [this]() {
        qDebug() << "PACKETS PER SECOND:" << m_packetCounter;
        m_packetCounter = 0;
    });
    m_ppsTimer.start(1000);
}

void TcpManager::onMessageRecieved(Packet &packet)
{
    QDataStream stream(&packet.data, QIODevice::ReadOnly);
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

//        m_csvParser->appendChannelDataBatch(result.channel, data);
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
    m_tcpSocket = m_tcpServer->nextPendingConnection();
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &TcpManager::onReadyRead);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, [this]() {
        m_tcpSocket->deleteLater();
    });

    emit clientConnected();
}

void TcpManager::onReadyRead()
{
    if (m_tcpSocket->bytesAvailable() > 1320 * HEADER_SIZE) {
        qWarning() << "Buffer overflow detected. Clearing" << m_tcpSocket->bytesAvailable() << "bytes";
        m_tcpSocket->readAll();
        resetState();
        return;
    }

    QDataStream stream(m_tcpSocket);
    stream.setByteOrder(QDataStream::LittleEndian);

    while (!stream.atEnd()) {
        if (!m_headerReaded && m_tcpSocket->bytesAvailable() >= HEADER_SIZE) {
            stream >> m_header.version
                   >> m_header.msgType
                   >> m_header.zero
                   >> m_header.timeCreated
                   >> m_header.countBytes;

            m_dataSize = qMin<qint32>(m_header.countBytes, 10000);
            m_headerReaded = true;
        }

        if (m_headerReaded && m_dataSize > 0 && m_tcpSocket->bytesAvailable() >= m_dataSize) {
            m_msgBytes.resize(m_dataSize);
            stream.readRawData(m_msgBytes.data(), m_dataSize);

            auto &provider = SequentialIdProvider::get();
            Packet packet(m_header, m_msgBytes, provider.next());
            onMessageRecieved(packet);

            resetState();
        } else {
            break;
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
    qInfo() << "Клиент отключился от сервера";
}
