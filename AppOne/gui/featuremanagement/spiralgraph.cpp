#include "spiralgraph.h"

#include <QDebug>

#include "src/common.h"

using namespace View;

SpiralGraph::SpiralGraph(QWidget *parent)
    : QCustomPlot(parent)
{
    setBackground(QBrush(QColor("#F2F0F0")));
    setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    m_spiralCurve = new QCPCurve(xAxis, yAxis);

    QPen spiralPen(QColor(40, 110, 255), 2);
    m_spiralCurve->setPen(spiralPen);
    m_spiralCurve->setScatterStyle(QCPScatterStyle::ssNone);
    m_spiralCurve->setLineStyle(QCPCurve::lsLine);

    xAxis->setLabel("Азимут (град)");
    yAxis->setLabel("Угол места (град)");
    xAxis->grid()->setSubGridVisible(true);
    yAxis->grid()->setSubGridVisible(true);
}

void SpiralGraph::setData(QVector<TargetDesignation> &targets)
{
    m_spiralCurve->data().clear();
    clearGraphs();

    if (targets.isEmpty()) {
        qDebug() << "Empty targets data";
        return;
    }

    QVector<double> x, y;
    x.resize(targets.count());
    y.resize(targets.count());

    for (int i = 0; i < targets.count(); i++) {
        x[i] = targets[i].azimut;
        y[i] = targets[i].elev;
    }

    m_spiralCurve->setData(x, y);

    rescaleAxes();
    replot();
}
