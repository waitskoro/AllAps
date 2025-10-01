#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

struct TargetDesignation;

namespace View {

class SpiralGraph;

class SpiralTabTargets : public QWidget
{
    Q_OBJECT
public:
    explicit SpiralTabTargets(QWidget *parent = nullptr);
    QVector<TargetDesignation> coordinates();

signals:

private:
    void init();
    // void paintEvent(QPaintEvent *event);

    void calculateFromInputs();
    void calculate(const double spiralStep,
                   const double plotLimit,
                   const double step);

    QLineEdit *m_step;
    QLineEdit *m_plotLimit;
    QLineEdit *m_spiralStep;

    SpiralGraph *m_graph;
    QPushButton *m_calcButton;
    QVector<TargetDesignation> *m_targets;
};

}
