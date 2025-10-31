#include "graphtracer.h"

GraphTracer::GraphTracer(QCPGraph* graph, QObject *parent)
    : QObject(parent)
    , m_graph(graph)
    , m_plotter(graph->parentPlot())
{
    connect(m_plotter, &QCustomPlot::mousePress,
            this, &GraphTracer::onMousePress);
}

double GraphTracer::getValue()
{
    return m_markers.last()->position->value();
}

double GraphTracer::getKey()
{
    return m_markers.last()->position->key();
}

void GraphTracer::setColor(QColor color)
{
    m_color = color;
}

void GraphTracer::setPosition(double pos)
{
    QPen pen(m_color);
    pen.setWidthF(3);

    QCPItemTracer *tracer = new QCPItemTracer(m_plotter);
    tracer->setGraph(m_graph);
    tracer->setStyle(QCPItemTracer::tsSquare);
    tracer->setPen(pen);
    tracer->setBrush(QBrush(m_color));

    m_markers.push_back(tracer);

    // Номерной заголовок для маркера
    QCPItemText *textLabel = new QCPItemText(m_plotter);
    textLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    textLabel->position->setParentAnchor(tracer->position);
    textLabel->position->setCoords(10, 0);
    textLabel->setText(QString::number(m_markers.count()));
    textLabel->setPen(QPen(Qt::darkRed)); // Цвет рамки и текста
    textLabel->setBrush(Qt::white); // Цвет фона
    textLabel->setPadding(QMargins(2, 1, 2, 1)); // Отступы внутри рамки

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

    m_dataMarkers.clear();

    m_markers.clear();
    m_plotter->replot();
}

void GraphTracer::onMousePress(QMouseEvent *event)
{
    if (!m_usage)
        return;

    int x = event->pos().x();
    int y = event->pos().y();

    if (event->button() == Qt::RightButton) {
        setPositionByPixel(x, y);
        return;
    }
}

void GraphTracer::setUsage(bool usage)
{
    m_usage = usage;
}

QVector<DataMarker> GraphTracer::getAllMarkersData()
{
    return m_dataMarkers;
}

void GraphTracer::onDataUpdate()
{
    if (m_markers.isEmpty())
        return;

    while (m_dataMarkers.size() < m_markers.size()) {
        m_dataMarkers.push_back(DataMarker());
    }

    int countData = 0;

    for (int i = 0; i < m_markers.count(); i++) {
        if (m_dataMarkers[i].data.isEmpty()) {
            for (int j = 0; j < countData; j++) {
                m_dataMarkers[i].data.push_back("");
            }
        } else {
            countData = m_dataMarkers[i].data.count();
        }

        m_dataMarkers[i].data.push_back(QString::number(m_markers[i]->position->value()));
    }
}
