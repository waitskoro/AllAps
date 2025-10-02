#pragma once

#include <QWidget>
#include <QVector>
#include <QTableView>
#include <QPushButton>

#include "datetarget.h"
#include "src/featuremanagment/suntargetdesignations.h"

struct TargetDesignation;

namespace View {

class CoordinatesConverter;
class TargetDesignationModel;

class SunTabTargets : public QWidget
{
    Q_OBJECT
public:
    explicit SunTabTargets(QWidget *parent = nullptr);

    void onButtonCreate(Dates);
    QVector<TargetDesignation> coordinates();

signals:
    void dateGeting();
    void sendTargets(QVector<Target>);
private:
    void init();
    void paintEvent(QPaintEvent *event);

    QTableView *m_tableView;

    QLineEdit *m_lat;
    QLineEdit *m_lon;
    QPushButton *m_lonConverter;
    QPushButton *m_latConverter;

    QPushButton *m_btnClear;
    QPushButton *m_buttonCreate;

    TargetDesignationModel *m_model;
    CoordinatesConverter *m_coordsConverter;
    SunTargetDesignations *m_sunTargetParser;
};

}
