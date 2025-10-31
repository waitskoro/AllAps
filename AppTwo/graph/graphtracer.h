#pragma once

#include <QObject>

#include "qcustomplot/qcustomplot.h"

struct DataMarker {
    QColor color;
    QVector<QString> data;
};

class GraphTracer : public QObject
{
    Q_OBJECT
public:
    explicit GraphTracer(QCPGraph* graph, QObject *parent = nullptr);

public slots:
    void reset();

    double getKey();
    double getValue();
    bool getUsage() { return m_usage; }
    QVector<DataMarker> getAllMarkersData();

    void setUsage(bool usage);
    void setColor(QColor color);
    void setPosition(double pos);
    void setPositionByPixel(double x, double y, int r = 30);

    void onDataUpdate();
    void onMousePress(QMouseEvent *evt);

private:
    bool m_usage = false;
    QColor m_color = "red";
    QCPGraph *m_graph = nullptr;
    QCustomPlot *m_plotter = nullptr;

    QVector<QCPItemTracer*> m_markers;
    QVector<DataMarker> m_dataMarkers;
};
