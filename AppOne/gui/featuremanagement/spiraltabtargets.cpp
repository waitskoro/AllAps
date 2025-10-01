#include "spiraltabtargets.h"
#include <QDebug>
#include <QLabel>
#include <QGridLayout>

#include "src/common.h"
#include "spiralgraph.h"

using namespace View;

SpiralTabTargets::SpiralTabTargets(QWidget *parent)
    : QWidget{parent}
    , m_step(new QLineEdit("0.1", this))
    , m_plotLimit(new QLineEdit("3.0", this))
    , m_spiralStep(new QLineEdit("0.5", this))
    , m_graph(new SpiralGraph(this))
    , m_calcButton(new QPushButton("Построить спираль", this))
    , m_targets(new QVector<TargetDesignation>())
{
    init();

    connect(m_calcButton, &QPushButton::clicked, this, &SpiralTabTargets::calculateFromInputs);
}

QVector<TargetDesignation> SpiralTabTargets::coordinates()
{
    return *m_targets;
}

void SpiralTabTargets::calculateFromInputs()
{
    bool ok;
    double step = m_step->text().toDouble(&ok);
    if (!ok || step <= 0) step = 0.1;

    double plotLimit = m_plotLimit->text().toDouble(&ok);
    if (!ok || plotLimit <= 0) plotLimit = 3.0;

    double spiralStep = m_spiralStep->text().toDouble(&ok);
    if (!ok || spiralStep <= 0) spiralStep = 0.5;

    calculate(spiralStep, plotLimit, step);
    m_graph->setData(*m_targets);
}

void SpiralTabTargets::init()
{
    QGridLayout *mainLayout = new QGridLayout(this);
    QGridLayout *leftLayout = new QGridLayout();
    QGridLayout *rightLayout = new QGridLayout();

    mainLayout->addLayout(leftLayout, 0, 0);
    mainLayout->addLayout(rightLayout, 0, 1);

    // График
    leftLayout->addWidget(m_graph);
    m_graph->setMinimumSize(300, 250);

    // Параметры
    rightLayout->addWidget(new QLabel("Шаг по времени:", this), 0, 0);
    rightLayout->addWidget(m_step, 0, 1);

    rightLayout->addWidget(new QLabel("Границы по осям:", this), 1, 0);
    rightLayout->addWidget(m_plotLimit, 1, 1);

    rightLayout->addWidget(new QLabel("Шаг между витками:", this), 2, 0);
    rightLayout->addWidget(m_spiralStep, 2, 1);

    rightLayout->addWidget(m_calcButton, 3, 0, 1, 2);
}

void SpiralTabTargets::calculate(const double spiralStep,
                                 const double plotLimit,
                                 const double step)
{
    const double k = spiralStep / (2 * M_PI);
    const double maxAngle = plotLimit / k;

    const int numPoints = static_cast<int>(maxAngle / step) + 1;

    m_targets->clear();
    m_targets->reserve(numPoints);

    double minAzimut = 0.0;
    double minElev = 0.0;

    QVector<double> azimuts;
    QVector<double> elevs;
    azimuts.reserve(numPoints);
    elevs.reserve(numPoints);

    for (int n = 0; n < numPoints; ++n) {
        const double t = n * step;

        double elev = k * (sin(t) - t * cos(t));
        double azimut = k * (cos(t) + t * sin(t));

        azimuts.append(azimut);
        elevs.append(elev);

        if (azimut < minAzimut) minAzimut = azimut;
        if (elev < minElev) minElev = elev;
    }

    double azimutOffset = fabs(minAzimut);
    double elevOffset = fabs(minElev);

    for (int n = 0; n < numPoints; ++n) {
        TargetDesignation target;
        target.azimut = azimuts[n] + azimutOffset;
        target.elev = elevs[n] + elevOffset;
        m_targets->append(target);
    }

    qDebug() << "Generated" << m_targets->size() << "points for spiral";
    qDebug() << "Offset applied - azimut:" << azimutOffset << "elev:" << elevOffset;
}
