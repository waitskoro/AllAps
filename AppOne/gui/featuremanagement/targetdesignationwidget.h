#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QTableView>
#include <QPushButton>
#include <QDateTimeEdit>

#include "src/featuremanagment/targetdesignationsinfo.h"

namespace View {

class DateTarget;
class GeneralInfoTarget;
class TargetDesignations;

class TargetDesignationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TargetDesignationWidget(QWidget *parent);
    ~TargetDesignationWidget();

signals:
    void goBack();
    void createTarget(Application::TargetDesignations m_target);
    void createTargetTest(Application::TargetDesignations m_target);
private:
    QPushButton *m_exit;
    QPushButton *m_create;
    QPushButton *m_testButton;

    DateTarget *m_dates;
    TargetDesignations *m_targets;
    GeneralInfoTarget *m_generalInfo;

    void initUI();
    void onTargetSend();

    void paintEvent(QPaintEvent *event);
};

}
