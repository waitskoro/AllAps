#include "plotter.h"

#include "qcustomplot.h"

using namespace View::Graphic;

Plotter::Plotter(QCustomPlot *parent)
    : QCustomPlot(parent)
{
    addGraph();
    graph(0)->setPen(QPen(QColor(40, 110, 255)));

    setInteraction(QCP::iRangeZoom,true);
    setInteraction(QCP::iRangeDrag, true);
    axisRect()->setRangeDrag(Qt::Horizontal);
    axisRect()->setRangeZoom(Qt::Horizontal);

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    xAxis->setTicker(timeTicker);
}
