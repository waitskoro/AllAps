#pragma once

#include <QWidget>

#include "qcustomplot.h"

struct TargetDesignation;

namespace View {

class SpiralGraph : public QCustomPlot
{
    Q_OBJECT
public:
    explicit SpiralGraph(QWidget *parent = nullptr);

    void setData(QVector<TargetDesignation> &targets);

signals:

private:
    QCPCurve *m_spiralCurve;
};

}
