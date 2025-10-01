#pragma once

#include "gui/graph/qcustomplot.h"

namespace {

qint64 convertToDateTime(double time)
{
    const double millisecondsPerDay = 86400.0 * 1000.0;
    double msSinceEpoch = time * millisecondsPerDay;
    return static_cast<qint64>(msSinceEpoch + 0.5);

}

void setupDateTimeTicker(QCPAxis* axis, const QString& format = "mm:ss")
{
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat(format);
    axis->setTicker(dateTicker);
}

}
