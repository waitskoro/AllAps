#include "plotter.h"

#include <complex>

#include "qcustomplot.h"
#include "src/graph/spectrumanalyzer.h"

using namespace View::Graphic;

Plotter::Plotter(QWidget *parent)
    : QCustomPlot(parent)
    , m_channels(new QVector<qint8>)
    , m_analyzer(new SpectrumAnalyzer())
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

void Plotter::addChannels(const QVector<qint8> channels)
{
    m_channels->clear();
    m_channels->append(channels);
}

void Plotter::addItem(const Report &msg, Type type)
{
    if (!m_channels->contains(msg.dataChannelNumber))
        return;

    double scale = 1.0 / 128.0;

    QVector<std::complex<double>> iqData;
    for (const auto& sample : msg.info) {
        iqData.emplace_back(sample[0] * scale, sample[1] * scale);
    }

    QVector<double> x, y;

    switch (type) {
        case Type::Power: {
            for (int i = 0; i < iqData.size(); ++i) {
                x.append(msg.time + i * 1.0 / iqData.size());
                y.append(std::norm(iqData[i]));

                if (std::norm(iqData[i]) > m_maxPower)
                    m_maxPower = std::norm(iqData[i]);
            }
            graph(msg.dataChannelNumber)->addData(x, y);
            break;
        }
        case Type::Signal: {
            for (int i = 0; i < iqData.size(); ++i) {
                x.append(msg.time + i * 1.0 / iqData.size());
                y.append(iqData[i].real());

                if (iqData[i].real() > m_maxSignal)
                    m_maxSignal = iqData[i].real();
            }
            graph(msg.dataChannelNumber)->addData(x, y);
            break;
        }
        case Type::Spectrum: {
            graph(msg.dataChannelNumber)->data().data()->clear();
            auto spectrum = m_analyzer->computeSpectrum(iqData);

            QVector<double> amplitudes(spectrum.size());
            for (int i = 0; i < spectrum.size(); ++i) {
                amplitudes[i] = std::abs(spectrum[i]);
            }

            int N = spectrum.size();
            for (int i = 0; i < N; ++i) {
                int shiftedIndex = (i + N / 2) % N;
                x.append(i - N / 2);
                y.append(amplitudes[shiftedIndex]);

                if (amplitudes[shiftedIndex] > m_maxSpectrum)
                    m_maxSpectrum = amplitudes[shiftedIndex];

            }
            graph(msg.dataChannelNumber)->addData(x, y);
            break;
        }
    }

    if (type == Type::Signal) {
        QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
        dateTicker->setDateTimeFormat("hh:mm:ss");
        xAxis->setTicker(dateTicker);
        yAxis->setLabel("Amplitude");
    } else if (type == Type::Spectrum) {
        xAxis->setLabel("Frequency (bins)");
        yAxis->setLabel("Magnitude");
    } else if (type == Type::Power) {
        QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
        dateTicker->setDateTimeFormat("hh:mm:ss");
        xAxis->setTicker(dateTicker);
        yAxis->setLabel("Power");
    }

    rescaleAxes();
    replot();
}

