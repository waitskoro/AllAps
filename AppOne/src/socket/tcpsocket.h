#pragma once

#include <QUrl>
#include <QTimer>
#include <QObject>
#include <QTcpSocket>

#include "src/messagesinfo.h"

namespace Application {

class TcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent = nullptr);

    void abortSocket();
    void disconnectFromHost();
    void connectToHost(const QUrl& url);
    void send(const QByteArray& header, const QByteArray& msg);

    bool isConnected() const;
    QAbstractSocket::SocketState state() const;

signals:
    void connected();
    void connectionTakingTooLong();
    void msgReceived(const Packet& packet);

private slots:
    void onConnectionTimeout();
    void onConnected();

private:
    qint32 m_headerSize = 16;

    QUrl m_url;
    QTcpSocket* m_socket;
    QTimer* m_connectionTimeoutTimer;

    void handleReadyRead();
    Header deserializeHeader(const QByteArray& data) const;

    bool header_readed = false;
    QByteArray header_bytes;
    QByteArray msg_bytes;
    Header header_;
    int data_size;
};

}
