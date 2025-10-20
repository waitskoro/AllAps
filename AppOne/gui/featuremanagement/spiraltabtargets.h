#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QDoubleSpinBox>

#include "src/common.h"

struct TargetDesignation;

namespace View {

class SpiralGraph;
class TargetDesignationModel;

class SpiralTabTargets : public QWidget
{
    Q_OBJECT
public:
    explicit SpiralTabTargets(TargetDesignationModel *model, QWidget *parent = nullptr);

private:
    void init();

    void calculateFromInputs();
    void calculate(const double spiralStep,
                   const double plotLimit,
                   const double step,
                   const double leftOffset,
                   const double bottomOffset);

    TargetDesignationModel *m_model;

    QDoubleSpinBox *m_step;
    QDoubleSpinBox *m_plotLimit;
    QDoubleSpinBox *m_spiralStep;
    QDoubleSpinBox *m_leftOffset;
    QDoubleSpinBox *m_bottomOffset;

    SpiralGraph *m_graph;
    QPushButton *m_calcButton;
    QPushButton *m_cleanButton;

    QVector<TargetDesignation> m_valuesGraph;
};

}
