#include "powerplotter.h"

PowerPlotter::PowerPlotter(QCustomPlot* plotter, QObject *parent)
    : QObject(parent)
{
    this->plot = plotter;

    ampDistGraph = plot->addGraph();
    plot->xAxis->setLabel("f, Гц");
    plot->yAxis->setLabel("дБ");
    plot->setInteractions(QCP::iRangeDrag |
                          QCP::iRangeZoom |
                          QCP::iSelectPlottables);
    ampDistGraph->setSelectable(QCP::stSingleData);

    ampDistTracer = new GraphTracer(ampDistGraph, this);

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("hh:mm:ss");
    plotter->xAxis->setTicker(dateTicker);

    processTimer = new QTimer(this);
    processTimer->setInterval(1000);
    connect(processTimer, &QTimer::timeout, this, &PowerPlotter::processCurrentData);
    processTimer->start();
}

bool PowerPlotter::isRescale()
{
    return useAutRescale;
}

void PowerPlotter::autoRescaleEnable()
{
    useAutRescale = !useAutRescale;
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

    ampDistGraph->setData(keys, values);
    ampDistGraph->rescaleAxes();
    plot->replot();

    currentSecondValues.clear();

    emit ampDistDataUpdate();
    ampDistTracer->onDataUpdate();
}
