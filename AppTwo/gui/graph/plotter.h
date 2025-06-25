#pragma once

#include <QWidget>

#include "qcustomplot.h"

class QCPGraph;

namespace View::Graphic {

class Plotter : public QCustomPlot
{
    Q_OBJECT
public:
    explicit Plotter(QCustomPlot *parent = nullptr);

signals:

// private:
    // QCustomPlot *customPlot;
};

}
