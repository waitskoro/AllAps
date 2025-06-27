#include "plotter.h"

#include <complex>
#include "qcustomplot.h"

using namespace View::Graphic;

Plotter::Plotter(QWidget *parent)
    : QCustomPlot(parent)
    , m_channels(new QList<qint8>)
{
    for (int i = 0; i <= 12; i++)
        addGraph();

    graph(0)->setPen(QPen(Qt::red));
    graph(1)->setPen(QPen(Qt::green));
    graph(2)->setPen(QPen(Qt::blue));
    graph(3)->setPen(QPen(Qt::cyan));
    graph(4)->setPen(QPen(Qt::magenta));
    graph(5)->setPen(QPen(Qt::yellow));
    graph(6)->setPen(QPen(Qt::darkRed));
    graph(7)->setPen(QPen(Qt::darkGreen));
    graph(8)->setPen(QPen(Qt::darkBlue));
    graph(9)->setPen(QPen(Qt::darkCyan));
    graph(10)->setPen(QPen(Qt::darkMagenta));
    graph(11)->setPen(QPen(Qt::darkYellow));

    setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
}

void Plotter::addChannels(const QList<qint8> channels)
{
    m_channels->clear();
    m_channels->append(channels);
}

void Plotter::addItem(const Report &msg, Type type)
{
    if (!m_channels->contains(msg.dataChannelNumber))
        return;

    double scale = 1.0/128.0;

    std::vector<std::complex<double>> iq_data;
    for (const auto& sample : msg.info) {
        iq_data.emplace_back(sample[0] * scale, sample[1] * scale);
    }

    QVector<double> x, y;

    switch (type) {
        case Type::I: {
            for (size_t i = 0; i < iq_data.size(); ++i) {
                x.append(msg.time + i * 1.0 / (iq_data.size()));
                y.append(iq_data[i].imag());
            }
            break;
        }
        case Type::Q: {
            for (size_t i = 0; i < iq_data.size(); ++i) {
                x.append(msg.time + i * 1.0 / (iq_data.size()));
                y.append(iq_data[i].real());
            }
            break;
        }
        case Type::Spectrum: {
            for (size_t i = 0; i < iq_data.size(); ++i) {
                x.append(msg.time + i * 1.0/(iq_data.size()));
                y.append(std::abs(iq_data[i]));
            }
            break;
        }
    }

    graph(msg.dataChannelNumber)->addData(x, y);

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("dd.MM.yyyy\nhh:mm:ss");
    xAxis->setTicker(dateTicker);

    rescaleAxes();

    replot();

    graph(msg.dataChannelNumber)->rescaleAxes(true);
}

