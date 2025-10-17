#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

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

    void roundData();
    void calculateFromInputs();
    void calculate(const double spiralStep,
                   const double plotLimit,
                   const double step);

    TargetDesignationModel *m_model;

    QLineEdit *m_step;
    QLineEdit *m_plotLimit;
    QLineEdit *m_spiralStep;

    SpiralGraph *m_graph;
    QPushButton *m_calcButton;
    QPushButton *m_cleanButton;

    QVector<TargetDesignation> m_valuesGraph;
};

}
