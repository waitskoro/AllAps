#pragma once

#include "server/countmessage.h"

#include <QObject>
#include <QListView>
#include <QStandardItemModel>

namespace View {

class InformationList : public QListView
{
    Q_OBJECT

public:
    explicit InformationList(QWidget *parent = nullptr);

    void addInfo(ChannelData &data);

private:
    QScopedPointer<QStandardItemModel> m_model;
};

}
