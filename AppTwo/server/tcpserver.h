#pragma once

#include "packet.h"

#include <QTimer>
#include <QThread>
#include <QObject>
#include <QSettings>
#include <QTcpServer>
#include <QTcpSocket>
#include <QCoreApplication>

struct c_pair{
    qint8 I;
    qint8 Q;
};
class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);

    bool isStarted() const;
    void startSending();

signals:
    void connected();
    void clientConnected();

    void dataRecieved(QByteArray);

private slots:
    void onClientDataReady();
    void onClientConnecting();
    void onClientDisconnected();

    void onMsgReceived(const Header&, const QByteArray&);

private:
    bool m_serverStarted;
    bool m_headerReaded;

    bool header_readed = false;
    QByteArray header_bytes;
    QByteArray msg_bytes;
    Header header_;
    int data_size;

    QTcpServer* m_server;
    QTcpSocket* m_socket;

    Header deserializeHeader(QByteArray& data);
};
