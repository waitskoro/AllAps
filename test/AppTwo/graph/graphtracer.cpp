#include "graphtracer.h"

GraphTracer::GraphTracer(QCPGraph* graph, QObject *parent)
    : QObject(parent)
{
    this->graph = graph;
    this->plot = graph->parentPlot();

    connect(plot, &QCustomPlot::mousePress,
            this, &GraphTracer::onMousePress);
}

double GraphTracer::getValue()
{
    return tracers.last()->position->value();
}

double GraphTracer::getKey()
{
    return tracers.last()->position->key();
}

void GraphTracer::setColor(QColor color)
{
    this->color = color;
}

void GraphTracer::setPosition(double pos)
{
    QPen pen(color);
    pen.setWidthF(3);

    QCPItemTracer *tracer = new QCPItemTracer(plot);
    tracer->setGraph(graph);
    tracer->setStyle(QCPItemTracer::tsPlus);
    tracer->setPen(pen);
    tracer->setBrush(QBrush(color));

    tracers.push_back(tracer);

    tracer->setGraphKey(pos);
    plot->replot();
    emit positionUpdate();
    emit valueUpdate();
}

void GraphTracer::setPositionByPixel(int x, int y, int r)
{
    double keyBegin = plot->xAxis->pixelToCoord(x - r);
    double keyEnd = plot->xAxis->pixelToCoord(x + r);
    double valueBegin = plot->yAxis->pixelToCoord(y + r);
    double valueEnd = plot->yAxis->pixelToCoord(y - r);

    r = r * 2;
    double key = 0.;
    double value = 0.;

    auto dataIter = graph->data()->begin();
    for (;dataIter != graph->data()->end(); ++dataIter) {
        if( dataIter->key < keyBegin || dataIter->key > keyEnd ) continue;
        if( dataIter->value < valueBegin || dataIter->value > valueEnd ) continue;

        int dotPosX = static_cast<int>(plot->xAxis->coordToPixel(dataIter->key));
        int dotPosY = static_cast<int>(plot->yAxis->coordToPixel(dataIter->value));

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
    if ( tracers.isEmpty() ) return;

    tracers.clear();
    plot->replot();
}

void GraphTracer::onMousePress(QMouseEvent *event)
{
    int x = event->pos().x();
    int y = event->pos().y();

    if (event->button() == Qt::RightButton) {
        setPositionByPixel(x, y);
        return;
    }
}

void GraphTracer::onDataUpdate()
{
    if (tracers.isEmpty()) return;
    emit valueUpdate();
}
