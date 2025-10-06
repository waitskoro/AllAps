#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QUrl>
#include "../messagesinfo.h"

namespace Application {

class TcpSocket : public QObject
{
    Q_OBJECT

public:
    explicit TcpSocket(QObject *parent = nullptr);
    ~TcpSocket();

    void disconnectFromHost();
    bool isConnected() const;
    void connectToHost(const QUrl& url);
    void send(const QByteArray& header, const QByteArray& msg);
    QAbstractSocket::SocketState state() const;

    // Метод для управления автоматическим переподключением
    void setAutoReconnect(bool enable) { m_autoReconnect = enable; }
    bool autoReconnect() const { return m_autoReconnect; }

signals:
    void connected();
    void msgReceived(Packet packet);
    void remoteHostClosed();
    void errorOccurred(const QString &errorString);
    void connectionTakingTooLong();

private slots:
    void onConnectionTimeout();
    void onConnected();
    void handleReadyRead();
    void onSocketStateChanged(QAbstractSocket::SocketState state);
    void onSocketError(QAbstractSocket::SocketError error);

private:
    Header deserializeHeader(const QByteArray& data) const;
    void resetReadState();
    void safeConnectToHost();
    void scheduleReconnect();

private:
    QTcpSocket* m_socket;
    QTimer* m_connectionTimeoutTimer;
    QTimer* m_reconnectTimer;
    QUrl m_url;

    qint32 m_headerSize = 16;

    bool header_readed = false;
    QByteArray header_bytes;
    QByteArray msg_bytes;
    Header header_;
    int data_size;

    bool m_autoReconnect = true;
};

}
