#include "plotter.h"

#include "graphtracer.h"
#include "common/functions.h"

using namespace View::Graphic;

Plotter::Plotter(Type type, QWidget *parent)
    : QCustomPlot(parent)
    , m_lazyUpdateTimer(new QTimer(this))
{
    setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    QColor graphColors[12] = {
        QColor(31, 119, 180),    // 1. Синий
        QColor(255, 127, 14),    // 2. Оранжевый
        QColor(44, 160, 44),     // 3. Зеленый
        QColor(214, 39, 40),     // 4. Красный
        QColor(148, 103, 189),   // 5. Фиолетовый
        QColor(140, 86, 75),     // 6. Коричневый
        QColor(227, 119, 194),   // 7. Розовый
        QColor(23, 190, 207),    // 8. Бирюзовый
        QColor(188, 189, 34),    // 9. Оливковый
        QColor(247, 182, 210),   // 10. Светло-розовый
        QColor(127, 127, 127),   // 11. Серый
        QColor(199, 199, 199)    // 12. Светло-серый
    };

    for (int i = 0; i < MAX_CHANNELS; ++i) {
        QCPGraph* mainGraph = addGraph();
        QPen mainPen(graphColors[i % 12]);
        mainPen.setWidthF(2.0);
        mainGraph->setPen(mainPen);
        mainGraph->setName(QString("Канал %1 (ср.)").arg(i+1));
        m_ampDistGraphs.append(mainGraph);

        QCPGraph* maxGraph = addGraph();
        QColor maxColor = graphColors[i % 12].darker(120);
        QPen maxPen(maxColor);
        maxPen.setWidthF(1.5);
        maxPen.setStyle(Qt::DashLine);
        maxGraph->setPen(maxPen);
        maxGraph->setName(QString("Канал %1 (макс.)").arg(i+1));
        m_maxGraphs.append(maxGraph);

        QCPGraph* minGraph = addGraph();
        QColor minColor = graphColors[i % 12].lighter(150);
        QPen minPen(minColor);
        minPen.setWidthF(1.5);
        minPen.setStyle(Qt::DotLine);
        minGraph->setPen(minPen);
        minGraph->setName(QString("Канал %1 (мин.)").arg(i+1));
        m_minGraphs.append(minGraph);

        m_ampDistTracers.append(new GraphTracer(mainGraph, this));
    }

    switch (type) {
        case Q:
            yAxis->setLabel("Q");
            xAxis->setLabel("t, сек.");
            setupDateTimeTicker(xAxis);
            break;
        case I: {
            yAxis->setLabel("I");
            xAxis->setLabel("t, сек.");
            setupDateTimeTicker(xAxis);
            break;
        }
        case Spectrum: {
            yAxis->setLabel("дБ");
            xAxis->setLabel("f, Гц");
            break;
        }
        case Power: {
            yAxis->setLabel("f, Гц");
            xAxis->setLabel("t, сек.");

            QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
            dateTicker->setDateTimeFormat("hh:mm:ss");
            xAxis->setTicker(dateTicker);

            break;
        }
    }

    m_lazyUpdateTimer->setInterval(100);
    connect(m_lazyUpdateTimer, &QTimer::timeout, [this]() {
        replot();
    });

    m_lazyUpdateTimer->start();
}

void Plotter::addData(const int channel, const double power, double time)
{
    if (channel < 1 || channel > MAX_CHANNELS)
        return;

    trimData(channel, m_countPowerPoints);

    int index = channel - 1;
    m_ampDistGraphs[index]->addData(convertToDateTime(time) / 1000.0, power);
    m_ampDistGraphs[index]->rescaleAxes();
    m_countPowerPoints[channel]++;
}

void Plotter::addData(const int channel, const ChannelStats stats, const double datetime)
{
    if (channel < 1 || channel > MAX_CHANNELS)
        return;

    trimData(channel, m_countPowerPoints);
    trimData(channel, m_countMaxPoints);
    trimData(channel, m_countMinPoints);

    int index = channel - 1;
    double time = convertToDateTime(datetime) / 1000.0;

    m_ampDistGraphs[index]->addData(time, stats.avg);
    m_maxGraphs[index]->addData(time, stats.max);
    m_minGraphs[index]->addData(time, stats.min);

    rescaleAxes();

    m_countPowerPoints[channel]++;
    m_countMaxPoints[channel]++;
    m_countMinPoints[channel]++;
}

void Plotter::setData(const int channel, const QVector<double> &freq, const QVector<double> &amp)
{
    if (channel < 1 || channel > MAX_CHANNELS)
        return;

    int index = channel - 1;
    clearAllGraphs();

    for (int i = 0; i < freq.size() && i < amp.size(); i++) {
        m_ampDistGraphs[index]->addData(freq[i], amp[i]);
    }
    m_ampDistTracers[index]->onDataUpdate();
    rescaleAxes();
}

void Plotter::addData(Type type, const int channel, const ChannelStats stats, const double datetime)
{
    if (channel < 1 || channel > MAX_CHANNELS)
        return;

    int index = channel - 1;
    double time = convertToDateTime(datetime) / 1000.0;

    if (type == Q) {
        trimData(channel, m_countQPoints);
        trimData(channel, m_countMaxPoints);
        trimData(channel, m_countMinPoints);
        m_countQPoints[channel]++;
    }
    else if (type == I) {
        trimData(channel, m_countIPoints);
        trimData(channel, m_countMaxPoints);
        trimData(channel, m_countMinPoints);
        m_countIPoints[channel]++;
    }

    m_ampDistGraphs[index]->addData(time, stats.avg);
    m_maxGraphs[index]->addData(time, stats.max);
    m_minGraphs[index]->addData(time, stats.min);

    if (type == Q || type == I) {
        double currentTime = convertToDateTime(datetime) / 1000.0;
        xAxis->setRange(currentTime - 10, currentTime);
    }

    yAxis->rescale();
    replot();
}

void Plotter::trimData(int channel, QMap<int, int> &countPoints)
{
    int index = channel - 1;
    QCPGraph* graph = nullptr;

    if (&countPoints == &m_countPowerPoints ||
        &countPoints == &m_countQPoints ||
        &countPoints == &m_countIPoints) {
        graph = m_ampDistGraphs[index];
    }
    else if (&countPoints == &m_countMaxPoints) {
        graph = m_maxGraphs[index];
    }
    else if (&countPoints == &m_countMinPoints) {
        graph = m_minGraphs[index];
    }

    if (!graph || countPoints[channel] < MAX_POINTS)
        return;

    int pointsToRemove = static_cast<int>(countPoints[channel] * 0.4);
    QSharedPointer<QCPGraphDataContainer> newData(new QCPGraphDataContainer);

    auto it = graph->data()->constBegin() + pointsToRemove;
    auto end = graph->data()->constEnd();

    for (; it != end; ++it)
        newData->add(*it);

    graph->setData(newData);
    countPoints[channel] -= pointsToRemove;
    graph->removeFromLegend();
}

void Plotter::clearAllGraphs()
{
    for (int i = 0; i < MAX_CHANNELS; i++) {
        if (m_ampDistGraphs.size() > i) m_ampDistGraphs[i]->data()->clear();
        if (m_maxGraphs.size() > i) m_maxGraphs[i]->data()->clear();
        if (m_minGraphs.size() > i) m_minGraphs[i]->data()->clear();
    }
}
