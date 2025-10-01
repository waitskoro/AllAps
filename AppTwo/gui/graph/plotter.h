#pragma once

#include <QMap>
#include <QWidget>
#include <complex>

#include "qcustomplot.h"
#include "common/enums.h"
#include "common/messages.h"

class QCPGraph;

class GraphTracer;

namespace View::Graphic {

typedef std::complex<double> Complex;
typedef QVector<Complex> ComplexVector;

class Plotter : public QCustomPlot
{
    Q_OBJECT

public:
    explicit Plotter(Type, QWidget *parent = nullptr);

    void clearAllGraphs();

    void addData(const int channel, const double power, double time);
    void addData(const int channel, const ChannelStats, const double datetime);
    void setData(const int channel, const QVector<double> &freq, const QVector<double> &amp);
    void addData(Type type, const int channel, const ChannelStats, const double datetime);

private:
    void trimData(int channel, QMap<int, int> &countPoints);

    static constexpr int MAX_CHANNELS = 12;
    static constexpr int MAX_POINTS = 50000;

    QTimer *m_lazyUpdateTimer;

    QMap<int, int> m_countMaxPoints;
    QMap<int, int> m_countMinPoints;

    QMap<int, int> m_countQPoints;
    QMap<int, int> m_countIPoints;
    QMap<int, int> m_countPowerPoints;

    QVector<QCPGraph*> m_maxGraphs;
    QVector<QCPGraph*> m_minGraphs;
    QVector<QCPGraph*> m_ampDistGraphs;

    QVector<GraphTracer*> m_ampDistTracers;
};

}
