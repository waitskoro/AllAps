#pragma once

#include <QObject>
#include <complex>

namespace Ui {
class MainWindow;
}

class Dsp;
class DftPlotter;
class TimePlotter;
class PowerPlotter;

class GraphManager : public QObject
{
    Q_OBJECT
public:
    explicit GraphManager(QObject *parent = nullptr);

    void setUi(Ui::MainWindow *ui);
    void onSamplesReaded(QVector<std::complex<double> > dataComplex);

private:
    Dsp *m_dsp;
    Ui::MainWindow *m_ui;
    TimePlotter *m_plotterI;
    TimePlotter *m_plotterQ;
    DftPlotter *m_dftPlotter;
    PowerPlotter *m_powerPlotter;
};

