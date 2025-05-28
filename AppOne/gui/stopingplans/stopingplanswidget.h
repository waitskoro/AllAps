#pragma once

#include <QLabel>
#include <QWidget>
#include <QObject>
#include <QPushButton>

#include "src/messagesinfo.h"

namespace View {

class StopingPlansWidget : public QWidget
{
    Q_OBJECT

    enum State {
        Question,
        Sending,
        Sended,

    };

public:
    explicit StopingPlansWidget(QWidget *parent = nullptr);

    void showWindow();
    void messageRecivied(ExecutedTheCommand result);
signals:
    void send();
    void stoped();
    void canceled();

private:
    void paintEvent(QPaintEvent *e);

    QLabel *m_label;
    QRect *m_insideRect;
    QPushButton *m_cancel;
    QPushButton *m_buttonOk;

    bool m_isStoped;

};

}

