#include "timeplotter.h"

TimePlotter::TimePlotter(QString title, QCustomPlot* plotter, QObject *parent) : QObject(parent)
{
    this->plot = plotter;

    iGraph =  plot->addGraph(plot->xAxis, plot->yAxis);

    plot->xAxis->setLabel("t, сек.");
    plot->yAxis->setLabel(title);
    plot->setInteractions(QCP::iRangeDrag |
                          QCP::iRangeZoom);
}

void TimePlotter::setData(const QVector<double> &time, const QVector<double> &i)
{
    iGraph->setData(time, i);
    if (useAutRescale) {
        iGraph->rescaleAxes();
        plot->replot();
        emit iDataUpdate();
    }
}

void TimePlotter::autoRescaleEnable()
{
    useAutRescale = !useAutRescale;
}
