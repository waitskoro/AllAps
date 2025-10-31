#pragma once

#include <QObject>

#include "plotter.h"
#include "graphtracer.h"
#include "../qcustomplot/qcustomplot.h"

class PowerPlotter : public Plotter
{
    Q_OBJECT
public:
    explicit PowerPlotter(QCustomPlot* plotter, QObject *parent = nullptr);

    void clearData(int channel);
    void setRangeGraph(int range);
    void setCurrentChannel(int channel);
    void addData(int channel, double power);

private:
    int m_range = 60; // seconds
    int m_currentChannel = 1;

    QTimer* m_processTimer;
    QMap<double, double> m_dataMap;

    double avg(const QVector<double>& vector);
    void processCurrentData();

    QMap<int, QVector<double>> m_currentSecondValues;
    QMap<int, QVector<std::pair<double, double>>> m_powerOnChannel;
};
