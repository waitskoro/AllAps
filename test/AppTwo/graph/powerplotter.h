#pragma once

#include <QObject>

#include "graphtracer.h"
#include "../qcustomplot/qcustomplot.h"

class PowerPlotter : public QObject
{
    Q_OBJECT
public:
    explicit PowerPlotter(QCustomPlot* plotter, QObject *parent = nullptr);

    bool isRescale();
    void clearData();
    void setRangeGraph(int range);
    void setCurrentChannel(int channel);
    void addData(int channel, double power);

public slots:
    void autoRescaleEnable();

public:
    QCustomPlot* plot;
    QCPGraph* distGraph;
    GraphTracer* distTracer;

private:
    int m_range = 60; // seconds
    bool m_autoRescale = true;
    int m_currentChannel = 1;

    QTimer* m_processTimer;
    QMap<double, double> m_dataMap;

    double avg(QVector<double>);
    void processCurrentData();

    QMap<int, QVector<double>> m_currentSecondValues;
    QMap<int, QVector<std::pair<double, double>>> m_powerOnChannel;
};
