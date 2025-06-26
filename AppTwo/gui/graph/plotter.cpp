#include "plotter.h"

#include <complex>
#include "qcustomplot.h"

using namespace View::Graphic;

Plotter::Plotter(QWidget *parent)
    : QCustomPlot(parent)
{
    addGraph();

    setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
}

void Plotter::addItem(const Report &msg, Type type)
{
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

    graph(0)->addData(x, y);

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("dd.MM.yyyy\nhh:mm:ss");
    xAxis->setTicker(dateTicker);

    rescaleAxes();

    replot();

    graph(0)->rescaleAxes(true);
}

