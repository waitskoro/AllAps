#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QTableView>
#include <QPushButton>
#include <QDateTimeEdit>

#include "src/featuremanagment/targetdesignationsinfo.h"

namespace View {

class TargetDesignationTable;

class TargetDesignationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TargetDesignationWidget(QWidget *parent);
    ~TargetDesignationWidget();

    void paintEvent(QPaintEvent *event);

    QPushButton *m_exit;
    QPushButton *m_create;

signals:
    void goBack();
    void createTarget(Application::TargetDesignations m_target);

private:
    QComboBox *m_numberChannelData;
    QLineEdit *m_spacecraftNumber;
    QLineEdit *m_centerFrequency;

    QDateEdit *m_startDate;
    QTimeEdit *m_startTime;
    QDateEdit *m_endDate;
    QTimeEdit *m_endTime;

    TargetDesignationTable *m_coordinatesView;

    QComboBox *m_polarization;

    void initUI();
    void onTargetSend();
};

}
