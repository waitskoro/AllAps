#include "markersplotter.h"

#include <QModelIndex>
#include <QAbstractListModel>

#include "ui_framespector.h"

MarkersPlotter::MarkersPlotter(QCustomPlot* plotter, QObject *parent)
    : Plotter(plotter, parent)
    , m_tracer(new GraphTracer(getGraph(), this))
{
    connect(m_tracer,
            &GraphTracer::markerChanged,
            this,
            &MarkersPlotter::markerChanged);
}

void MarkersPlotter::clearMarkers()
{
    m_tracer->reset();
}
