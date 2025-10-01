#pragma once

#include <QMap>
#include <QWidget>
#include <QVector>
#include <QComboBox>

#include <complex>

#include "../dsp.h"
#include "qcustomplot.h"
#include "common/enums.h"
#include "common/messages.h"

namespace Graph {
class DftPlotter;
class TimePlotter;
class PowerPlotter;
}

class QLabel;
class QVBoxLayout;
class QHBoxLayout;

typedef std::complex<double> Complex;
typedef QVector<Complex> ComplexVector;

class GraphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GraphWidget(QWidget* parent = nullptr);
    ~GraphWidget();

    // set ------------------------
    void setChannel(int channel);

    // get ------------------------
    int channel() { return m_channel; };
    bool channelMatch(int channel);
    void recreateWindow(int size);
    void onSamplesReaded(QVector<std::complex<double> > dataComplex);

private:
    void init();

    Dsp dsp;
    int m_channel = 1;

    bool recreateWindowNext = true;
    double m_dftGraphAmpTracerMax = 0;
    double m_dftGraphAmpTracerMin = 0;
    double m_dftGraphAmpTracerAvg = 0;
    double m_dftGraphAmpTracerCrt = 0;

    QVBoxLayout* m_layout;
    QHBoxLayout* m_hLayout;

    QComboBox *m_channelData;

    Graph::DftPlotter *m_dftPlotter;
    Graph::TimePlotter *m_timePlotterI;
    Graph::TimePlotter *m_timePlotterQ;
    Graph::PowerPlotter *m_powerPlotter;
};
