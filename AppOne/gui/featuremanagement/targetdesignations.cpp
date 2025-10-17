#include "targetdesignations.h"

#include <QPainter>
#include <QTabWidget>

#include "suntabtargets.h"
#include "rangetabtargets.h"
#include "manualtabtargets.h"
#include "spiraltabtargets.h"

#include "src/featuremanagment/targetdesignationmodel.h"

using namespace View;

TargetDesignations::TargetDesignations(QWidget *parent)
    : QWidget(parent)
    , m_tabWidget(new QTabWidget(this))
    , m_model(new TargetDesignationModel(this))
    , m_sunTab(new SunTabTargets(m_model, this))
    , m_rangeTab(new RangeTabTargets(m_model, this))
    , m_manualTab(new ManualTabTargets(m_model, this))
    , m_spiralTab(new SpiralTabTargets(this)) // Пока списка указаний нет
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
    return m_model->coordinates();
}
