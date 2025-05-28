#pragma once

#include <QTabWidget>
#include <QPushButton>
#include <QMainWindow>

class TcpServer;

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

    QTabWidget *m_tabWidget;
    ConnectionHeader *m_connectionHeader;
    QPushButton *m_menu;

    int m_iconRotationAngle = 90;

    TcpServer *m_server;
};

}
