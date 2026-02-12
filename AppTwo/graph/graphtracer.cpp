#include "graphtracer.h"

GraphTracer::GraphTracer(QCPGraph* graph, QObject *parent)
    : QObject(parent)
    , m_graph(graph)
    , m_plotter(graph->parentPlot())
{
    connect(m_plotter,
            &QCustomPlot::mousePress,
            this,
            &GraphTracer::onMousePress);

    connect(m_plotter,
            &QCustomPlot::afterReplot,
            this,
            &GraphTracer::onReplot);
}

void GraphTracer::onReplot()
{
    for (int i = 0; i < m_markers.size(); i++) {
        auto *tracer = m_markers[i];

        if (tracer && tracer->graph()) {
            emit markerChanged(i, QString::number(tracer->position->value(), 'f', 2));
        }
    }
}

void GraphTracer::setPosition(double pos)
{
    QPen pen("red");
    pen.setWidthF(3);

    QCPItemTracer *tracer = new QCPItemTracer(m_plotter);
    tracer->setGraph(m_graph);
    tracer->setStyle(QCPItemTracer::tsSquare);
    tracer->setPen(pen);
    tracer->setBrush(QBrush(pen.color()));

    m_markers.push_back(tracer);

    QCPItemText *textLabel = new QCPItemText(m_plotter);
    textLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    textLabel->position->setParentAnchor(tracer->position);
    textLabel->position->setCoords(10, 10);

    textLabel->setText(QString::number(m_markers.size()));
    textLabel->setPen(QPen(Qt::darkRed));
    textLabel->setBrush(Qt::white);
    textLabel->setPadding(QMargins(2, 1, 2, 1));
    textLabel->setPadding(QMargins(4, 3, 4, 3));

    tracer->setGraphKey(pos);
    m_plotter->replot();
}

void GraphTracer::setPositionByPixel(double x, double y, int r)
{
    double keyBegin = m_plotter->xAxis->pixelToCoord(x - r);
    double keyEnd = m_plotter->xAxis->pixelToCoord(x + r);
    double valueBegin = m_plotter->yAxis->pixelToCoord(y + r);
    double valueEnd = m_plotter->yAxis->pixelToCoord(y - r);

    r = r * 2;
    double key = 0.;
    double value = 0.;

    auto dataIter = m_graph->data()->begin();
    for (;dataIter != m_graph->data()->end(); ++dataIter) {
        if (dataIter->key < keyBegin || dataIter->key > keyEnd) continue;
        if (dataIter->value < valueBegin || dataIter->value > valueEnd) continue;

        int dotPosX = static_cast<int>(m_plotter->xAxis->coordToPixel(dataIter->key));
        int dotPosY = static_cast<int>(m_plotter->yAxis->coordToPixel(dataIter->value));

        // Манхэттенское расстояние
        int rCur = std::abs(dotPosX - x) + std::abs(dotPosY - y);

        if (rCur < r) {
            r = rCur;
            key = dataIter->key;
            value = dataIter->value;
        }
    }

    if (key == 0. && value == 0.) {
        reset();
        return;
    }

    setPosition(key);
}

void GraphTracer::reset()
{
    if (m_markers.isEmpty())
        return;

    for (auto *marker : m_markers)
        marker->deleteLater();

    m_markers.clear();
    m_plotter->replot();
}

void GraphTracer::onMousePress(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        int x = event->pos().x();
        int y = event->pos().y();
        setPositionByPixel(x, y);
    }
}
