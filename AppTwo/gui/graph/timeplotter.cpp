#include "timeplotter.h"

using namespace Graph;

TimePlotter::TimePlotter(QObject *parent)
    : IPlotter(parent)
    , plotter(new QCustomPlot())
    , graph(plotter->addGraph(plotter->xAxis, plotter->yAxis))
{
    plotter->xAxis->setLabel("t, сек.");
    plotter->setInteractions(QCP::iRangeDrag |
                             QCP::iRangeZoom);
}

void TimePlotter::setData(const QVector<double> &time, const QVector<double> &i)
{
    graph->setData(time, i);
    graph->rescaleAxes();
    plotter->replot();
    emit dataUpdated();
}
