#pragma once

#include <QObject>
#include <QVector>

#include "qcustomplot.h"
#include "graphtracer.h"

namespace Graph {

class IPlotter : public QObject
{
    Q_OBJECT
public:
    explicit IPlotter(QObject *parent = nullptr)
        : QObject(parent)
    {}

public slots:
    virtual void setData(const QVector<double> &first, const QVector<double> &second) = 0;

signals:
    void dataUpdated();

};

}
