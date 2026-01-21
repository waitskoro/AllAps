#pragma once

#include <QFrame>
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

    bool getRescale();

    void setHistorySize(int value);
    void setMinLevel(double value);
    void setMaxLevel(double value);
    void setColorScheme(const QString &text);
    void addSpectrum(const QVector<double>& spectrum);

    void setVisibleWidget(int index, bool visible);
    void setData(const QVector<double> &freq, const QVector<double> &ampDist);

private:
    Ui::FrameSpector *ui;

    Spectrogram *m_spectrogram;
    MarkersPlotter *m_plotter;
    QStandardItemModel *m_model;

    void updateSpectrogram();
    void onMarkerChanged(int index, QString value);
};

