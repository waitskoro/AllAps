#include "powerplotter.h"

PowerPlotter::PowerPlotter(QCustomPlot* plotter, QObject *parent)
    : QObject(parent)
{
    this->plot = plotter;

    distGraph = plot->addGraph();
    plot->xAxis->setLabel("f, Гц");
    plot->yAxis->setLabel("дБ");
    plot->setInteractions(QCP::iRangeDrag |
                          QCP::iRangeZoom |
                          QCP::iSelectPlottables);

    distGraph->setSelectable(QCP::stSingleData);
    distTracer = new GraphTracer(distGraph, this);

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("hh:mm:ss");
    plotter->xAxis->setTicker(dateTicker);

    m_processTimer = new QTimer(this);
    m_processTimer->setInterval(1000);

    connect(m_processTimer,
            &QTimer::timeout,
            this,
            &PowerPlotter::processCurrentData);
}

bool PowerPlotter::isRescale()
{
    return m_autoRescale;
}

void PowerPlotter::autoRescaleEnable()
{
    m_autoRescale = !m_autoRescale;
}

void PowerPlotter::clearData()
{
    m_dataMap.clear();
    m_currentSecondValues.clear();
    distGraph->data()->clear();

    distTracer->onDataUpdate();
    distGraph->rescaleAxes();
    plot->replot();
}

void PowerPlotter::setRangeGraph(int range)
{
    m_range = range * 60;

    double currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
    double startTime = currentTime - m_range;

    distGraph->keyAxis()->setRange(startTime, currentTime);
}

void PowerPlotter::setCurrentChannel(int channel)
{
    m_processTimer->stop();
    clearData();

    m_currentChannel = channel;

    for (auto& pair : m_powerOnChannel[m_currentChannel])
        distGraph->addData(pair.first, pair.second);

    distGraph->rescaleAxes();
    plot->replot();

    m_currentSecondValues.clear();
    distTracer->onDataUpdate();

    m_processTimer->start();
}

void PowerPlotter::addData(int channel, double power)
{
    if (!m_processTimer->isActive()) {
        m_processTimer->start();
    }

    m_currentSecondValues[channel].append(power);
}

double PowerPlotter::avg(QVector<double> vector)
{
    double sum = 0;

    for (double value : vector)
        sum += value;

    return sum / vector.size();
}

void PowerPlotter::processCurrentData()
{
    distGraph->data()->clear();

    // Обновление времени по всем каналам
    for (int i = 1; i <= 12; i++) {
        double avgPower = avg(m_currentSecondValues[i]);
        double date = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;

        m_powerOnChannel[i].append({date, avgPower});

        // Удаление первых элементов из сохранения если их больше часа
        while (m_powerOnChannel[i].size() > 3600) {
            auto it = m_powerOnChannel[i].begin();
            m_powerOnChannel[i].erase(it);
        }
    }

    m_currentSecondValues.clear();

    // Построение графика
    for (auto& pair : m_powerOnChannel[m_currentChannel])
        distGraph->addData(pair.first, pair.second);

    if (isRescale()) {
        double currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
        double startTime = currentTime - m_range;

        distGraph->keyAxis()->setRange(startTime, currentTime);
        distGraph->valueAxis()->rescale();
//        distGraph->rescaleAxes();
    }

    plot->replot();
    distTracer->onDataUpdate();
}
