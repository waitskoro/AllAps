#pragma once

#include "iplotter.h"
#include "qcustomplot.h"

#include <QObject>

namespace Graph {

class TimePlotter : public IPlotter
{
    Q_OBJECT
public:
    explicit TimePlotter(QObject *parent = nullptr);

    void setData(const QVector<double> &first, const QVector<double> &second) override;
    void setWindowData(const QVector<double> & time, const QVector<double> & w);

    QCustomPlot* plotter;
    QCPGraph* graph;
};

}
