#include "channeldatalist.h"

#include "channeldatadelegate.h"

using namespace View;

ChannelDataList::ChannelDataList(QWidget *parent)
    : QListView(parent)
    , m_model(new QStandardItemModel(this))
{
    auto *delegate = new ChannelDataDelegate(this);
    setItemDelegate(delegate);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setSpacing(3);
    setFixedSize(250, 400);
    setModel(m_model.get());
    setStyleSheet("background-color: #FFFFFF");

    close();

    connect(delegate,
            &ChannelDataDelegate::itemClicked,
            this,
            &ChannelDataList::itemClicked);
}

void ChannelDataList::clear()
{
    m_model->clear();
}

void ChannelDataList::addMessage(const DataChannelInfo &msg)
{
    auto *item = new QStandardItem();
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    item->setData(msg.segmentCount, SegmentCount);
    item->setData(msg.channelNumber, ChannelNumber);

    m_model->appendRow(item);
}
