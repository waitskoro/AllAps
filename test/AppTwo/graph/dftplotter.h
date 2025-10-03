#pragma once

#include <QObject>

#include "graphtracer.h"
#include "../qcustomplot/qcustomplot.h"

class DftPlotter : public QObject
{
    Q_OBJECT
public:
    explicit DftPlotter(QCustomPlot* plotter, QObject *parent = nullptr);

    bool isRescale();
    void autoRescaleEnable();
    void setData(const QVector<double> &freq, const QVector<double> &amp);

signals:
    void ampDistDataUpdate();

public:
    QCustomPlot* plot;
    QCPGraph* ampDistGraph;
    GraphTracer* ampDistTracer;
    bool useAutRescale = true;
};
