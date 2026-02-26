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

    void setVisibleWidget(int index, bool visible);
    bool getRescale() { return m_plotter->getRescale(); };
    void setData(const QVector<double> &freq, const QVector<double> &ampDist) {
        m_plotter->setData(freq, ampDist);
    }

private:
    Ui::FrameSpector *ui;

    MarkersPlotter *m_plotter;
    QStandardItemModel *m_model;

    void onMarkerChanged(int index, QString value);
};
