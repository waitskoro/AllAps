#pragma once

#include "src/statemessage.h"

#include <QListView>
#include <QStandardItemModel>

namespace View {

class CamRadiatorsList : public QListView
{
    Q_OBJECT
public:

    enum Emitter {
        Number = Qt::UserRole + 1,
        Pol,
        State,
        SignalLevel
    };

    explicit CamRadiatorsList(QWidget *parent = nullptr);

    void clear();
    void addRadiators(EmmiterState *state);

private:
    QScopedPointer<QStandardItemModel> m_model;
};

}
