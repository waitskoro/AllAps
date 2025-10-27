#pragma once

#include <QObject>

#include "qcustomplot/qcustomplot.h"

class GraphTracer : public QObject
{
    Q_OBJECT
public:
    explicit GraphTracer(QCPGraph* graph, QObject *parent = nullptr);

    double getValue();
    double getKey();

signals:
    void positionUpdate();
    void valueUpdate();

public slots:
    void setColor(QColor color);
    void setPosition(double pos);
    void setPositionByPixel(int x, int y, int r = 30);
    void reset();

    void onMousePress(QMouseEvent *evt);
    void onDataUpdate();

public:
    QColor color = "#FF5900";
    QCPGraph* graph = nullptr;
    QCustomPlot* plot = nullptr;
    QVector<QCPItemTracer*> tracers;
};
