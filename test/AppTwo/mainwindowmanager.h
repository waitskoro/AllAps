#pragma once

#include <QObject>
#include <complex>
#include <QStandardItemModel>

#include "src/message.h"

class InfoManager;
class GraphManager;
class InfoListDelegate;

namespace Ui {
class MainWindow;
}

typedef std::complex<double> Complex;
typedef QVector<Complex> ComplexVector;

class MainWindowManager : public QObject
{
    Q_OBJECT
public:
    explicit MainWindowManager(QObject *parent = nullptr);

    void setUi(Ui::MainWindow *ui);
    void setWidget(QWidget *widget);

    void onServerCreated();
    void onCountMessageRecieved(const Report &);

signals:
    void serverStopped();
    void createServer(const int port);

private:
    InfoManager *m_infoManager;
    GraphManager *m_graphManager;

    QWidget *m_widget;
    Ui::MainWindow *m_ui;

    ComplexVector convertToComplex(const QVector<std::array<qint8, 2>> &data);
};

