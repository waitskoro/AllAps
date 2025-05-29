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
    enum Cdo {
        State,
        Amperage,
        Voltage,
        Temperature,
        CdoState,
        CamCount
    };

    explicit StateWorkingList(QWidget *parent = nullptr);
    void addState(CdoMessage &state);

signals:
    void itemClicked(int);

private:
    QScopedPointer<QStandardItemModel> m_model;
};

}
