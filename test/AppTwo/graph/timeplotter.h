#pragma once

#include <QObject>

#include "../qcustomplot/qcustomplot.h"

class TimePlotter : public QObject
{
    Q_OBJECT
public:
    explicit TimePlotter(QString title, QCustomPlot* plotter, QObject *parent = nullptr);

    bool isRescale();

signals:
    void iDataUpdate();

public slots:
    void autoRescaleEnable();
    void setData(const QVector<double> & time, const QVector<double> & i);

public:
    QCustomPlot* plot;
    QCPGraph* iGraph;
    bool useAutRescale = true;
};
