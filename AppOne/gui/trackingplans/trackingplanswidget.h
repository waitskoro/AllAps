#pragma once

#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QPushButton>

#include "src/channelmessage.h"

namespace View {

class ChannelDataList;

class TrackingPlansWidget : public QWidget
{
    Q_OBJECT

    enum State {
        None,
        Waiting,
        SuccessfullData,
        UnsuccessfullData
    };

public:
    explicit TrackingPlansWidget(QWidget *parent = nullptr);

    void addMessage(DataChannelMessage &);

signals:
    void goBack();

private:
    void initUI();
    void paintEvent(QPaintEvent *event);

    QLabel *m_label;
    QPushButton *m_exit;
    QLabel *m_headerLabel;

    State m_currentState;

    ChannelDataList *m_channelDataTable;
};

}
