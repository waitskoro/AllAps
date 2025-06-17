#pragma once

#include <QWidget>
#include <QPushButton>

#include "gui/mainwidget.h"

namespace View {

class MenuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent = nullptr);

signals:
    void sendFeature(CurrentView);
    void disconnect();

private:
    void paintEvent(QPaintEvent *e);

    QPushButton *m_stateWorking;
    QPushButton *m_trackingPlans;

    QPushButton *m_target;
    QPushButton *m_stopingPlans;

    QPushButton *m_disconnect;

    QPushButton *m_main;
};

}
