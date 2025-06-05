#pragma once

#include "common/messages.h"

#include <QLabel>
#include <QMainWindow>

namespace View {

class LineEdit;
class InfoWidget;
class ServerConnectingWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void onServerCreated();
    void onClientConnected();
    void onCountMessageRecieved(Count);

signals:
    void createServer(const QString& ip, const int port);

private:
    void init();
    void paintEvent(QPaintEvent *e) override;

    QLabel *m_label;
    InfoWidget *m_infoViewer;
    ServerConnectingWidget *m_serverConnecting;
};

}
