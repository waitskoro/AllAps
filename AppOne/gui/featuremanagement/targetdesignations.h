#pragma once

#include "datetarget.h"
#include "src/common.h"

#include <QWidget>
#include <QTabWidget>

namespace View {

class SunTabTargets;
class RangeTabTargets;
class ManualTabTargets;
class SpiralTabTargets;

class TargetDesignations : public QWidget
{
    Q_OBJECT
public:
    explicit TargetDesignations(QWidget *parent = nullptr);

    int beam();
    void setDates(Dates);
    QVector<TargetDesignation> coordinates();

signals:
    void dateGeting();
private:
    void init();
    void paintEvent(QPaintEvent *event);

    QTabWidget *m_tabWidget;
    SunTabTargets *m_sunTab;
    RangeTabTargets *m_rangeTab;
    ManualTabTargets *m_manualTab;
    SpiralTabTargets *m_spiralTab;
};

}
