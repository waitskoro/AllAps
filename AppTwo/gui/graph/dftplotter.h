#pragma once

#include <QObject>
#include "iplotter.h"
#include "graphtracer.h"

namespace Graph {

class DftPlotter : public IPlotter
{
    Q_OBJECT
public:
    explicit DftPlotter(QObject *parent = nullptr);

    void setData(const QVector<double> &freq, const QVector<double> &amp) override;

    QCustomPlot *plotter;
    QCPGraph *graph;
    GraphTracer *tracer;
};

}
