#include "plotter.h"

#include "graphtracer.h"

Plotter::Plotter(QCustomPlot* plotter, QObject *parent)
    : QObject(parent)
    , m_plotter(plotter)
    , m_graph(m_plotter->addGraph())
{
    m_plotter->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void Plotter::clearGraph()
{
    m_graph->data()->clear();
    m_plotter->replot();
}

void Plotter::setData(const QVector<double> &x, const QVector<double> &y)
{
    m_graph->setData(x, y);

    if (m_rescale) {
        m_graph->rescaleAxes();
        m_plotter->replot();
    }

    emit dataUpdated();
}
