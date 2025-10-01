#include "powerplotter.h"

using namespace Graph;

PowerPlotter::PowerPlotter(QObject *parent)
    : QObject(parent)
    , plotter(new QCustomPlot())
    , graph(plotter->addGraph())
    , tracer(new GraphTracer(graph, this))
{
    plotter->setInteractions(QCP::iRangeDrag |
                               QCP::iRangeZoom |
                               QCP::iSelectPlottables);

    graph->setSelectable(QCP::stSingleData);

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("hh:mm:ss");
    plotter->xAxis->setTicker(dateTicker);

    processTimer = new QTimer(this);
    processTimer->setInterval(1000); // 1 секунда
    connect(processTimer, &QTimer::timeout, this, &PowerPlotter::processCurrentData);
    processTimer->start();
}

void PowerPlotter::addData(double power)
{
    currentSecondValues.append(power);
}

void PowerPlotter::processCurrentData() {
    if (currentSecondValues.isEmpty())
        return;

    double sum = 0;
    for (double value : currentSecondValues) {
        sum += value;
    }
    double average = sum / currentSecondValues.size();

    double key = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
    dataMap[key] = average;

    const int MAX_POINTS = 300;

    while (dataMap.size() > MAX_POINTS) {
        auto it = dataMap.begin();
        dataMap.erase(it);
    }

    QVector<double> keys, values;
    keys.reserve(dataMap.size());
    values.reserve(dataMap.size());

    for (auto it = dataMap.begin(); it != dataMap.end(); ++it) {
        keys.append(it.key());
        values.append(it.value());
    }

    graph->setData(keys, values);
    graph->rescaleAxes();
    plotter->replot();

    currentSecondValues.clear();

    emit dataUpdated();
    tracer->onDataUpdate();
}

void PowerPlotter::clearData()
{
    graph->data()->clear();
    plotter->replot();
}


