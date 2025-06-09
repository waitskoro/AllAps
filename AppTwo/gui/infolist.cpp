#include "infolist.h"

#include "common/enums.h"
#include "infolistdelegate.h"

using namespace View;

InformationList::InformationList(QWidget *parent)
    : QListView(parent)
    , m_model(new QStandardItemModel(this))
{
    auto *delegate = new InfoListDelegate(this);
    setItemDelegate(delegate);

    setFixedSize(650, 410);

    setSpacing(5);
    setModel(m_model.get());

    setStyleSheet("background-color: #E4E5FF");
}

void InformationList::addInfo(Report msg)
{
    QModelIndexList matches = m_model->match(m_model->index(0, 0),
                                             ChannelNumber,
                                             msg.dataChannelNumber,
                                             -1,
                                             Qt::MatchExactly);

    for (const QModelIndex &index : matches) {
        if (index.isValid()) {
            if (m_model->data(index, KaNumber).toInt() == msg.kaNumber) {
                QStandardItem* existingItem = m_model->itemFromIndex(index);
                if (existingItem) {
                    setData(existingItem, msg);

                    return;
                }
            }
        }
    }

    auto *item = new QStandardItem();
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    setData(item, msg);
    m_model->appendRow(item);
}

void InformationList::setData(QStandardItem *item, Report msg)
{
    item->setData(msg.dataChannelNumber, ChannelNumber);
    item->setData(msg.kaNumber, KaNumber);
    item->setData(msg.acState, AcState);
    item->setData(msg.count, Count);
    item->setData(msg.time, Time);
    item->setData(msg.az[0], Az_1);
    item->setData(msg.az[1], Az_2);

    QVariant variant;
    variant.setValue(msg.info);
    item->setData(variant, Info);
}
