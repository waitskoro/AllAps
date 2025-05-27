#pragma once

#include <QPushButton>
#include <QMainWindow>

namespace Tcp {
class TcpServer;
}

namespace View {

class ConnectionHeader;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void paintEvent(QPaintEvent *e) override;

    ConnectionHeader *m_connectionHeader;
    QPushButton *m_menu;

    int m_iconRotationAngle = 90;

    void onConnectTo(const QString &ip, const QString &port);

    Tcp::TcpServer *m_server;
};

}
