#include "channeldatalist.h"

#include "channeldatadelegate.h"

using namespace View;

ChannelDataList::ChannelDataList(QWidget *parent)
    : QListView(parent)
    , m_model(new QStandardItemModel(this))
{
    auto *delegate = new ChannelDataDelegate(this);
    setItemDelegate(delegate);

    setSpacing(5);
    setFixedSize(600, 400);
    setModel(m_model.get());
    setStyleSheet("background-color: #C3D7E4");
}

void ChannelDataList::addMessage(const DataChannelMessage &msg)
{
    auto *item = new QStandardItem();
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    item->setData(msg.activeChannelsCount, ActiveChannelsCount);

    m_model->appendRow(item);
}
