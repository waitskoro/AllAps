#pragma once

#include "src/statemessage.h"

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

namespace View {

class StateWorkingList : public QListView
{
    Q_OBJECT
public:
    explicit StateWorkingList(QWidget *parent = nullptr);
    void addState(StateMessage &state);

private:
    void paintEvent(QPaintEvent *event);

    QScopedPointer<QStandardItemModel> m_model;
};

}
