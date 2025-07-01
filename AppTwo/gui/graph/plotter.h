#pragma once

#include <QMap>
#include <QWidget>

#include "qcustomplot.h"
#include "common/messages.h"

class QCPGraph;
class SpectrumAnalyzer;

namespace View::Graphic {


class Plotter : public QCustomPlot
{
    Q_OBJECT
public:
    enum Type {
        Power,
        Signal,
        Spectrum
    };

    explicit Plotter(QWidget *parent = nullptr);

    void addChannels(const QVector<qint8>);
    void addItem(const Report &msg, Type);
    void setupGraph(const QString &name, const QColor &color);

    double maxPower() { return m_maxPower; }
    double maxSignal() { return m_maxSignal; }
    double maxSpectrum() { return m_maxSpectrum ;}

signals:

private:
    void convertToComplex();

    QVector<qint8> *m_channels;
    SpectrumAnalyzer *m_analyzer;

    double m_maxPower = 0.0;
    double m_maxSignal = 0.0;
    double m_maxSpectrum = 0.0;
};

}
