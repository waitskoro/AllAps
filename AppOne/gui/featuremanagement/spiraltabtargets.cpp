 #include "spiraltabtargets.h"
#include <QDebug>
#include <QLabel>
#include <QGridLayout>

#include "src/common.h"
#include "spiralgraph.h"
#include "src/featuremanagment/targetdesignationmodel.h"

using namespace View;

SpiralTabTargets::SpiralTabTargets(TargetDesignationModel *model, QWidget *parent)
    : QWidget{parent}
    , m_model(model)
    , m_step(new QDoubleSpinBox(this))
    , m_plotLimit(new QDoubleSpinBox(this))
    , m_spiralStep(new QDoubleSpinBox(this))
    , m_leftOffset(new QDoubleSpinBox(this))
    , m_bottomOffset(new QDoubleSpinBox(this))
    , m_graph(new SpiralGraph(this))
    , m_calcButton(new QPushButton("Построить спираль", this))
    , m_cleanButton(new QPushButton("Очистить", this))
{
    init();

    m_step->setValue(0.1);
    m_plotLimit->setValue(3.0);
    m_spiralStep->setValue(0.5);
    m_leftOffset->setRange(0, 360);
    m_bottomOffset->setRange(0, 360);

    connect(m_calcButton,
            &QPushButton::clicked,
            this,
            &SpiralTabTargets::calculateFromInputs);

    connect(m_cleanButton,
            &QPushButton::clicked,
            [this]() {
        m_model->clear();
        m_graph->clear();
    });
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

    double leftOffset = m_leftOffset->value();
    double bottomOffset = m_bottomOffset->value();

    calculate(spiralStep, plotLimit, step, leftOffset, bottomOffset);

    m_graph->setData(m_valuesGraph);
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

    rightLayout->addWidget(new QLabel("Смещение слева:", this), 3, 0);
    rightLayout->addWidget(m_leftOffset, 3, 1);

    rightLayout->addWidget(new QLabel("Смещение от низа:", this), 4, 0);
    rightLayout->addWidget(m_bottomOffset, 4, 1);

    rightLayout->addWidget(m_calcButton, 5, 0);
    rightLayout->addWidget(m_cleanButton, 5, 1);
}

void SpiralTabTargets::calculate(const double spiralStep,
                                 const double plotLimit,
                                 const double step,
                                 const double leftOffset,
                                 const double bottomOffset)
{
    m_valuesGraph.clear();

    const double k = spiralStep / (2 * M_PI);
    const double maxAngle = plotLimit / k;

    const int numPoints = static_cast<int>(maxAngle / step) + 1;

    double minAzimut = 0.0;
    double minElev = 0.0;

    QVector<double> azimuts;
    QVector<double> angles;
    azimuts.reserve(numPoints);
    angles.reserve(numPoints);

    for (int n = 0; n < numPoints; ++n) {
        const double t = n * step;

        double elev = k * (sin(t) - t * cos(t));
        double azimut = k * (cos(t) + t * sin(t));

        azimuts.append(azimut);
        angles.append(elev);

        if (azimut < minAzimut) minAzimut = azimut;
        if (elev < minElev) minElev = elev;
    }

    double azimutOffset = fabs(minAzimut) + leftOffset;
    double angleOffset = fabs(minElev) + bottomOffset;

    for (int n = 0; n < numPoints; ++n) {
        TargetDesignation target;
        target.azimut = azimuts[n] + azimutOffset;
        target.elev = angles[n] + angleOffset;

        m_valuesGraph.push_back(target);
        m_model->append(std::round(std::round(target.azimut * 10.0) / 10.0),
                        std::round(std::round(target.elev * 10.0) / 10.0));
    }

    qDebug() << "Generated" << m_model->coordinates().count() << "points for spiral";
}
