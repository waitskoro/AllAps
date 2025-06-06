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
    enum Cam {
        State = Qt::UserRole + 1,
        Amperage,
        Voltage,
        Temperature,
        CdoState,
        CamCount,
        CamInfo,
        SectorNumber
    };

    explicit StateWorkingList(QWidget *parent = nullptr);

    void clear();
    void addState(CdoMessage &state, int sector);

signals:
    void itemClicked(std::vector<CamState>, int sector);

private:
    QScopedPointer<QStandardItemModel> m_model;
};

}
