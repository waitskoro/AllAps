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
    void addData(double power);
    void clearData();
    void setTimeRange(int minutes);

signals:
    void ampDistDataUpdate();

public slots:
    void autoRescaleEnable();

public:
    QCustomPlot* plot;
    QCPGraph* ampDistGraph;
    GraphTracer* ampDistTracer;
    bool useAutRescale = true;

private:
    QTimer* processTimer;
    QMap<double, double> dataMap;
    QVector<double> currentSecondValues;

    void processCurrentData();
};
