#pragma once

#include <QObject>

#include "plotter.h"
#include "graphtracer.h"

namespace Ui {
class FrameSpector;
}

class MarkersPlotter : public Plotter
{
    Q_OBJECT
public:
    explicit MarkersPlotter(QCustomPlot* plotter, QObject *parent = nullptr);

    void clearMarkers();

signals:
    void markerChanged(int index, QString value);

private:
    GraphTracer *m_tracer;
};
