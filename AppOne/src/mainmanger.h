#pragma once

#include <QObject>

namespace View {
class MainWidget;
}

namespace Application {

class TcpSocketManager;

class MainManager : public QObject
{
    Q_OBJECT

public:
    explicit MainManager(TcpSocketManager *tcpManager,
                         QObject *parent = nullptr);

    void showWindow();
    void closeWindow();

signals:
    void reconnect();
    void disconnect();

private:
    TcpSocketManager *m_tcpManager;
    View::MainWidget *m_mainWidget;
};

}
