#pragma once

#include "src/statemessage.h"

#include <QLabel>
#include <QWidget>
#include <QPushButton>

namespace View {

class StateWorkingList;

class StateWorkingWidget : public QWidget
{
    Q_OBJECT

    enum StateRole {
        Computer,
        AC,
        Switch_1,
        Switch_2,
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
    void drawStates(StateRole, QPainter, QPoint, qint32 state);

    StateMessage m_state;

};

}
