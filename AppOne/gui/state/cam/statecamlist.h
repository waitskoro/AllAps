#pragma once

#include <QListView>
#include <QStandardItemModel>

#include "src/statemessage.h"

namespace View {

class StateCamList : public QListView
{
    Q_OBJECT
public:
    explicit StateCamList(QWidget *parent = nullptr);

    void addCam(CamState &state);
    void clear();

private:
    QScopedPointer<QStandardItemModel> m_model;
};

}
