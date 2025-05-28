#pragma once

#include <QUrl>
#include <QTimer>
#include <QObject>

#include "src/messagesinfo.h"
#include "src/featuremanagment/targetdesignationsinfo.h"

namespace Application {

class TcpSocket;

class TcpSocketManager : public QObject
{
    Q_OBJECT

    enum ConnectionStatus {
        Unconnected,
        Connecting,
        Connected,
        Disconnected
    };

public:
    explicit TcpSocketManager(QObject *parent = nullptr);

    void disconnect();
    void cancelConnecting();
    void connectToHosts(const QUrl &ac);

    void stopingPlans();
    void sendRequestStateWorking();
    void sendRequestTraskingPlans();
    void sendTarget(TargetDesignations target);

signals:
    void connected();

    void receivingMessageEmpty();
    void recieveState(RecieveState);
    void receivingMessage(ReceivingMessage);
    void recievedTrackingPlans(DataChannelMessage &);
    void executedTheCommandRecevied(ExecutedTheCommand result);

    void msgReceived(const Packet &packet);
    void stateChanged(ConnectionStatus status);

private:
    TcpSocket *m_socket;

    bool isSocketsConnected = false;

    void onConnected();

    void sendEmptyPacket(uint8_t);
    void onSocketAcPacketReceived(Packet packet);
};

}
