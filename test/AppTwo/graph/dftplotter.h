#pragma once

#include <QObject>

#include "graphtracer.h"
#include "../qcustomplot/qcustomplot.h"

class DftPlotter : public QObject
{
    Q_OBJECT
public:
    explicit DftPlotter(QCustomPlot* plotter, QObject *parent = nullptr);

signals:
    void ampDistDataUpdate();

public slots:
    void autoRescaleEnable();
    void setData(const QVector<double> &freq, const QVector<double> &amp);

public:
    QCustomPlot* plot;
    QCPGraph* ampDistGraph;
    GraphTracer* ampDistTracer;
    bool useAutRescale = true;
};
