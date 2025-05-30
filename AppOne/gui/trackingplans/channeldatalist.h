#pragma once

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "src/channelmessage.h"

namespace View {

class ChannelDataList : public QListView
{
    Q_OBJECT
public:

    enum Roles {
        SegmentCount  = Qt::UserRole + 1,
        ChannelNumber
    };

    explicit ChannelDataList(QWidget *parent = nullptr);

    void addMessage(const DataChannelInfo &);

private:
    QScopedPointer<QStandardItemModel> m_model;
};

}
