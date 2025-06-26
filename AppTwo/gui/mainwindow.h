#pragma once

#include "common/messages.h"

#include <QLabel>
#include <QMainWindow>

namespace View {

class LineEdit;
class InfoWidget;
class ServerConnectingWidget;

namespace Graphic {
class GraphWidget;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void onServerCreated();
    void onClientConnected();
    void onCountMessageRecieved(const Report &);

signals:
    void createServer(const int port);

private:
    void init();
    void paintEvent(QPaintEvent *e) override;

    QLabel *m_label;
    QTabWidget *m_tabWidget;

    InfoWidget *m_infoViewer;
    Graphic::GraphWidget *m_graphViewer;
    ServerConnectingWidget *m_serverConnecting;

    QTimer* m_reportTimer;  // Таймер для генерации отчетов
    double m_currentTime;   // Текущее время для упорядоченных дат

private slots:
    void generateReport();  // Слот для генерации отчета
};

}
