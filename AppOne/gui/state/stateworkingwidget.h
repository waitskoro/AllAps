#pragma once

#include "src/statemessage.h"

#include <QLabel>
#include <QWidget>
#include <QPushButton>

namespace View {

class StateCamList;
class StateWorkingList;

class StateWorkingWidget : public QWidget
{
    Q_OBJECT

    enum StateRole {
        Computer,
        AC,
        SwitchComputer,
        Frequency
    };

public:
    explicit StateWorkingWidget(QWidget *parent = nullptr);

    void addState(StateMessage &);

signals:
    void exit();

private:
    QLabel *m_label;
    QPushButton *m_exit;
    QLabel *m_headerLabel;

    StateWorkingList *m_stateWorkingList;

    void initUI();
    void paintEvent(QPaintEvent *event);
    void drawStates(StateRole, QPainter *, QPoint, qint32 state);

    QColor colorOnState(int state);

    StateMessage m_state;
    StateCamList *m_camList;
    QLabel *m_hidingRect;

    int m_camNum = 0;

};

}
