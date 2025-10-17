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
class TargetDesignationModel;

class TargetDesignations : public QWidget
{
    Q_OBJECT
public:
    explicit TargetDesignations(QWidget *parent = nullptr);

    int angle();
    void setDates(Dates);
    QVector<TargetDesignation> coordinates();

signals:
    void dateGeting();
private:
    void init();
    void paintEvent(QPaintEvent *event);

    QTabWidget *m_tabWidget;

    TargetDesignationModel *m_model;

    SunTabTargets *m_sunTab;
    RangeTabTargets *m_rangeTab;
    ManualTabTargets *m_manualTab;
    SpiralTabTargets *m_spiralTab;
};

}
