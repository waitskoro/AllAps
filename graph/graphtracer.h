#pragma once

#include <QObject>

#include "qcustomplot/qcustomplot.h"

class GraphTracer : public QObject
{
    Q_OBJECT
public:
    explicit GraphTracer(QCPGraph* graph, QObject *parent = nullptr);

    void reset();

signals:
    void markerChanged(int index, QString value);

private:
    QCPGraph *m_graph = nullptr;
    QCustomPlot *m_plotter = nullptr;

    QVector<QCPItemTracer*> m_markers;

private:
    void onReplot();
    void setPosition(double pos);
    void onMousePress(QMouseEvent *evt);
    void setPositionByPixel(double x, double y, int r = 30);
};
