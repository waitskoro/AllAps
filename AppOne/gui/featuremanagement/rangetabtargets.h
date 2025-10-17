#pragma once

#include <QWidget>
#include <QComboBox>
#include <QTableView>
#include <QPushButton>

#include "datetarget.h"

struct TargetDesignation;

namespace View {

struct Sector {
    int start;
    int end;
};

class TargetDesignationModel;

class RangeTabTargets : public QWidget
{
    Q_OBJECT
public:
    explicit RangeTabTargets(TargetDesignationModel *model, QWidget *parent = nullptr);

    void onButtonCreate();
    QVector<TargetDesignation> coordinates();

signals:

private:
    void init();
    void paintEvent(QPaintEvent *event);

    QTableView *m_tableView;
    QPushButton *m_btnClear;
    QPushButton *m_buttonCreate;

    QLineEdit *m_step;
    QLineEdit *m_beamStart;
    QLineEdit *m_azimutStart;

    QComboBox *m_increase;

    TargetDesignationModel *m_model;
    QVector<Sector> m_sectors;
};

}
