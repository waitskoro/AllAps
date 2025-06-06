#pragma once

#include <QListView>
#include <QStandardItemModel>

#include "src/statemessage.h"

namespace View {

class StateCamList : public QListView
{
    Q_OBJECT
public:
    enum Roles {
        State = Qt::UserRole + 1,
        Amperage,
        Temperature,
        EmmCount,
        EmmState
    };

    explicit StateCamList(QWidget *parent = nullptr);

    void addCam(CamState &state);
    void clear();

private:
    QScopedPointer<QStandardItemModel> m_model;
};

}
