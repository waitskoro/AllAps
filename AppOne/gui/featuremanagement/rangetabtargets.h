#pragma once

#include <QWidget>
#include <QComboBox>
#include <QTableView>
#include <QPushButton>
#include <QDoubleSpinBox>

#include "datetarget.h"

struct TargetDesignation;

namespace View {

class TargetDesignationModel;

class RangeTabTargets : public QWidget
{
    Q_OBJECT
public:
    explicit RangeTabTargets(TargetDesignationModel *model, QWidget *parent = nullptr);

    void onButtonCreate();

private:
    void init();
    void paintEvent(QPaintEvent *event);

    QTableView *m_tableView;

    QPushButton *m_btnClear;
    QPushButton *m_buttonCreate;
    QDoubleSpinBox *m_step;
    QDoubleSpinBox *m_angleStart;
    QDoubleSpinBox *m_azimutStart;
    QDoubleSpinBox *m_endParam;
    QComboBox *m_increase;

    TargetDesignationModel *m_model;
};

}
