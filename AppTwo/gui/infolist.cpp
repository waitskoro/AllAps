#include "infolist.h"

#include "common/enums.h"
#include "infolistdelegate.h"

using namespace View;

InformationList::InformationList(QWidget *parent)
    : QListView(parent)
    , isNew(false)
    , m_clearTimer(new QTimer(this))
    , m_refreshTimer(new QTimer(this))
    , m_model(new QStandardItemModel(this))
{
    auto *delegate = new InfoListDelegate(this);
    setItemDelegate(delegate);

    setFixedSize(650, 410);

    setSpacing(5);
    setModel(m_model.get());

    connect(m_refreshTimer.get(), &QTimer::timeout, this, [this]() {
        isNew = true;
    });

    m_clearTimer->setSingleShot(true);
    connect(m_clearTimer.get(), &QTimer::timeout, this, [this]() {
        m_model->clear();
    });

    m_refreshTimer->start(1500);

    setStyleSheet("background-color: #E4E5FF");
}

void InformationList::addInfo(Report msg)
{
    m_clearTimer->start(2000);

    if (isNew) {
        m_model->clear();
        isNew = false;
    }

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
