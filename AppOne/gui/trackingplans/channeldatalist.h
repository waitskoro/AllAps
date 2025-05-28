#pragma once

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "src/messagesinfo.h"

namespace View {

class ChannelDataList : public QListView
{
    Q_OBJECT
public:
    explicit ChannelDataList(QWidget *parent = nullptr);

    void addMessage(const DataChannelMessage &);

private:
    QScopedPointer<QStandardItemModel> m_model;
};

}
