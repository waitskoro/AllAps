#pragma once

#include <QFrame>

#include "graph/spectrogram.h"
#include "graph/markersplotter.h"

namespace Ui {
class FrameSpector;
}

class Spectrogram;
class MarkersPlotter;

class FrameSpector : public QFrame
{
    Q_OBJECT

public:
    explicit FrameSpector(QWidget *parent = nullptr);
    ~FrameSpector();

    void setVisibleWidget(int index, bool visible);

    bool getRescale() { return m_plotter->getRescale(); };
    void setHistorySize(int value) { m_spectrogram->setMaxHistory(value); }
    void setMinLevel(double value) { m_spectrogram->setMinLevel(value); }
    void setMaxLevel(double value) { m_spectrogram->setMaxLevel(value); }

    void setColorScheme(const QString &text) {
        m_spectrogram->setColorMap(text);
    }
    void addSpectrum(const QVector<double>& spectrum) {
        m_spectrogram->addSpectrum(spectrum);
    }

    void setData(const QVector<double> &freq, const QVector<double> &ampDist) {
        m_plotter->setData(freq, ampDist);
    }

private:
    Ui::FrameSpector *ui;

    Spectrogram *m_spectrogram;
    MarkersPlotter *m_plotter;
    QStandardItemModel *m_model;

    void updateSpectrogram();
    void onMarkerChanged(int index, QString value);
};

