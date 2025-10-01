// infolist.cpp
#include "infolist.h"
#include "common/enums.h"
#include "infolistdelegate.h"
#include <QDateTime>

using namespace View;

InformationList::InformationList(QWidget *parent)
    : QListView(parent),
    m_model(new QStandardItemModel(this)),
    m_delegate(new InfoListDelegate(this))
{
    setItemDelegate(m_delegate);
    setSpacing(5);
    setModel(m_model.get());
    setStyleSheet("background-color: #E4E5FF");

    connect(&m_cleanupTimer, &QTimer::timeout, this, &InformationList::checkDataFreshness);
    m_cleanupTimer.start(1000);
}

void InformationList::addInfo(const Report &msg)
{
    QModelIndexList matches = m_model->match(m_model->index(0, 0),
                                             ChannelNumber,
                                             msg.channel,
                                             -1,
                                             Qt::MatchExactly);

    for (const QModelIndex &index : matches) {
        if (index.isValid() &&
            m_model->data(index, KaNumber).toInt() == msg.kaNumber) {
            QStandardItem *existingItem = m_model->itemFromIndex(index);
            updateExistingItem(existingItem, msg);
            m_lastUpdateTimes[existingItem] = QDateTime::currentMSecsSinceEpoch();
            return;
        }
    }

    auto *item = new QStandardItem();
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    setData(item, msg);
    m_model->appendRow(item);
    m_lastUpdateTimes[item] = QDateTime::currentMSecsSinceEpoch();
}

void InformationList::checkDataFreshness()
{
    const qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    const qint64 timeout = 2000;

    QList<QStandardItem*> toRemove;

    for (auto it = m_lastUpdateTimes.begin(); it != m_lastUpdateTimes.end(); ++it) {
        if (currentTime - it.value() > timeout) {
            toRemove.append(it.key());
        }
    }

    foreach (QStandardItem* item, toRemove) {
        m_model->removeRow(item->row());
        m_lastUpdateTimes.remove(item);
    }
}

void InformationList::updateExistingItem(QStandardItem *item, const Report &msg)
{
    item->setData(msg.count, Count);
    item->setData(msg.time, Time);
    item->setData(msg.az[0], Az_1);
    item->setData(msg.az[1], Az_2);

    QVariant variant;
    variant.setValue(msg.info);
    item->setData(variant, Info);
}

void InformationList::setData(QStandardItem *item, const Report &msg)
{
    item->setData(msg.channel, ChannelNumber);
    item->setData(msg.kaNumber, KaNumber);
    item->setData(msg.acState, AcState);
    updateExistingItem(item, msg);
}
