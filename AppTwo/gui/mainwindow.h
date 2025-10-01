#pragma once

#include "dsp.h"
#include <complex>
#include "common/messages.h"

#include <QLabel>
#include <QMainWindow>
#include <QElapsedTimer>

class GraphWidget;

namespace View {

class LineEdit;
class InfoWidget;
class ServerConnectingWidget;

typedef std::complex<double> Complex;
typedef QVector<Complex> ComplexVector;

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
    void dataReceived(const Report &msg);
private:
    void init();
    void paintEvent(QPaintEvent *e) override;
    ComplexVector convertToComplex(const QVector<std::array<qint8, 2>> &data);

    double m_lastDateTime;
    bool m_newDataAvailable = false;

    QLabel *m_label;
    QTimer *m_timer;
    QTimer* m_dataTimeoutTimer;

    QTabWidget *m_tabWidget;
    InfoWidget *m_infoViewer;
    GraphWidget *m_graphViewer;
    ServerConnectingWidget *m_serverConnecting;

    QMap<int, double> m_avgSecQ;
    QMap<int, double> m_avgSecI;
    QMap<int, double> m_avgSecPower;
};

}
