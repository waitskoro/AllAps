#pragma once

#include <QObject>
#include <QDateTime>
#include "qcustomplot.h"
#include "graphtracer.h"

namespace Graph {

class PowerPlotter : public QObject
{
    Q_OBJECT

public:
    explicit PowerPlotter(QObject *parent = nullptr);

    void addData(double power);
    void clearData();
    void setTimeRange(int minutes);

    QCustomPlot* plotter;
    QCPGraph* graph;
    GraphTracer* tracer;

signals:
    void dataUpdated();

private:
    QTimer* processTimer;
    QMap<double, double> dataMap;
    QVector<double> currentSecondValues;

    void processCurrentData();
};

}
