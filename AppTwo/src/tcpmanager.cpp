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
}

void TcpManager::onMessageRecieved(Packet &packet)
{
    QDataStream stream(&packet.data, QIODevice::ReadOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    if (packet.header.msgType == 0x82) {
        Report result;
        stream >> result;

        emit countMessage(result);

        QVector<QPair<double, double>> data;
        data.reserve(result.count);
        for (quint32 i = 0; i < result.count; i++) {
            data.append({result.info[i][0], result.info[i][1]});
        }

        m_csvParser->appendChannelDataBatch(result.dataChannelNumber, data);
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
        qInfo() << QString("Сервер запущен. %1:%2").arg(anyAddressString).arg(port);
        emit serverCreated();
    }
}

void TcpManager::onClientConnected()
{
    m_tcpSocket = m_tcpServer->nextPendingConnection();

    m_tcpSocket->waitForBytesWritten(1000);
    m_tcpSocket->readAll();

    m_tcpSocket->setParent(nullptr);

    QThread* thread = new QThread();
    m_tcpSocket->moveToThread(thread);

    connect(thread, &QThread::started, [this]() {
        m_tcpSocket->waitForReadyRead(1000);
        m_tcpSocket->readAll();
    });

    connect(m_tcpSocket,
            &QTcpSocket::readyRead,
            this,
            &TcpManager::onReadyRead);

    connect(m_tcpSocket, &QTcpSocket::disconnected, [this, thread]() {
        m_tcpSocket->deleteLater();
        thread->quit();
        thread->wait();
        thread->deleteLater();
    });

    thread->start();
    emit clientConnected();
}

void TcpManager::onReadyRead()
{

    if (!m_headerReaded) {

        if(m_tcpSocket->bytesAvailable() >= HEADER_SIZE){
            m_headerBytes = m_tcpSocket->read(HEADER_SIZE);
            m_header = deserializeHeader(m_headerBytes);

            if (m_header.countBytes < 10000) {
                m_dataSize = m_header.countBytes;
            }
            m_headerReaded = true;
        }
    }

    if (m_headerReaded) {

        if (m_dataSize > 0) {

            QByteArray chunk = m_tcpSocket->read(qMin(m_dataSize, m_tcpSocket->bytesAvailable()));
            m_msgBytes += chunk;;
            m_dataSize -= chunk.size();
        }

        if (m_dataSize == 0){
            auto &provider = SequentialIdProvider::get();
            long long id = provider.next();

            Packet packet(m_header, m_msgBytes, id);

            onMessageRecieved(packet);
            m_headerReaded = false;
            m_msgBytes.clear();
            m_headerBytes.clear();
        }
    }
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
