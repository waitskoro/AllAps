#include "dftplotter.h"

using namespace Graph;

DftPlotter::DftPlotter(QObject *parent)
    : IPlotter(parent)
    , plotter(new QCustomPlot())
    , graph(plotter->addGraph())
    , tracer(new GraphTracer(graph, this))
{
    plotter->setInteractions(QCP::iRangeDrag |
                               QCP::iRangeZoom |
                               QCP::iSelectPlottables);

    graph->setSelectable(QCP::stSingleData);
}

void DftPlotter::setData(const QVector<double> &freq, const QVector<double> &amp)
{
    graph->setData(freq, amp);
    graph->rescaleAxes();
    plotter->replot();

    emit dataUpdated();
    tracer->onDataUpdate();
}
