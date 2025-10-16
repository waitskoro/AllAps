#include "targetdesignations.h"

#include <QPainter>
#include <QTabWidget>

#include "suntabtargets.h"
#include "rangetabtargets.h"
#include "manualtabtargets.h"
#include "spiraltabtargets.h"

using namespace View;

TargetDesignations::TargetDesignations(QWidget *parent)
    : QWidget(parent)
    , m_tabWidget(new QTabWidget(this))
    , m_sunTab(new SunTabTargets(this))
    , m_rangeTab(new RangeTabTargets(this))
    , m_manualTab(new ManualTabTargets(this))
    , m_spiralTab(new SpiralTabTargets(this))
{
    init();

    connect(m_sunTab, &SunTabTargets::dateGeting,
            this, &TargetDesignations::dateGeting);
}

void TargetDesignations::init()
{
    m_tabWidget->setFixedSize(600, 270);

    m_tabWidget->addTab(m_manualTab, "Ручной ввод");
    m_tabWidget->addTab(m_sunTab, "По солнцу");
    m_tabWidget->addTab(m_spiralTab, "Спираль");
    m_tabWidget->addTab(m_rangeTab, "По диапазону");
}

void TargetDesignations::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.fillRect(rect(), QColor("#C3D7E4"));
}

int TargetDesignations::beam()
{
    return m_manualTab->beam();
}

void TargetDesignations::setDates(Dates dates)
{
    m_sunTab->onButtonCreate(dates);
}

QVector<TargetDesignation> TargetDesignations::coordinates()
{
    if (m_tabWidget->currentIndex() == 0) {
        return m_manualTab->coordinates();
    } else if (m_tabWidget->currentIndex() == 1) {
        return m_sunTab->coordinates();
    } else if (m_tabWidget->currentIndex() == 2) {
        return m_spiralTab->coordinates();
    } else if (m_tabWidget->currentIndex() == 3) {
        return m_rangeTab->coordinates();
    }

    return {};
}
