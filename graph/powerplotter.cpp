#include "powerplotter.h"

PowerPlotter::PowerPlotter(QCustomPlot* plotter, QObject *parent)
    : Plotter(plotter, parent)
{
    setXLabel("f, Гц");
    setYLabel("дБ");

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("hh:mm:ss");
    setXTicker(dateTicker);

    m_processTimer = new QTimer(this);
    m_processTimer->setInterval(1000);

    connect(m_processTimer,
            &QTimer::timeout,
            this,
            &PowerPlotter::processCurrentData);
}

void PowerPlotter::clearData(int channel)
{
    m_powerOnChannel[channel].clear();
}

void PowerPlotter::setRangeGraph(int range)
{
    m_range = range * 60;

    double currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
    double startTime = currentTime - m_range;

    getGraph()->keyAxis()->setRange(startTime, currentTime);
}

void PowerPlotter::setCurrentChannel(int channel)
{
    m_processTimer->stop();

    clearGraph();

    m_currentChannel = channel;

    for (auto& pair : m_powerOnChannel[m_currentChannel])
        getGraph()->addData(pair.first, pair.second);

    getGraph()->rescaleAxes();
    getPlotter()->replot();

    m_currentSecondValues.clear();

    m_processTimer->start();
}

void PowerPlotter::addData(int channel, double power)
{
    if (!m_processTimer->isActive()) {
        m_processTimer->start();
    }

    m_currentSecondValues[channel].append(power);
}

double PowerPlotter::avg(const QVector<double>& vector)
{
    if (vector.isEmpty()) return 0.0;

    return std::accumulate(vector.begin(), vector.end(), 0.0) / vector.size();
}

void PowerPlotter::processCurrentData()
{
    if (m_currentSecondValues.isEmpty()) {
         m_processTimer->stop();
         return;
     }

    getGraph()->data()->clear();

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
        getGraph()->addData(pair.first, pair.second);

    if (getRescale()) {
        double currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
        double startTime = currentTime - m_range;

        getGraph()->keyAxis()->setRange(startTime, currentTime);
        getGraph()->valueAxis()->rescale();
    }

    getPlotter()->replot();
}
