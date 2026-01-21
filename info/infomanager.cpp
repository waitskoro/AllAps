#include "infomanager.h"

#include <QDateTime>

#include "enums.h"
#include "info/infolistdelegate.h"

InfoManager::InfoManager(QObject *parent)
    : QObject(parent)
    , m_delegate(new InfoListDelegate())
    , m_model(new QStandardItemModel(this))
{
    connect(&m_cleanupTimer,
            &QTimer::timeout,
            this,
            &InfoManager::checkDataFreshness);

    m_cleanupTimer.start(1000);
}

void InfoManager::addInfo(bool isQint16, const Report &msg)
{
    m_isQint16 = isQint16;

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

void InfoManager::setListView(QListView *listView)
{
    m_listView = listView;

    m_listView->setSpacing(5);
    m_listView->setModel(m_model.get());
    m_listView->setItemDelegate(m_delegate);
    m_listView->setStyleSheet("background-color: #E4E5FF");
}

void InfoManager::checkDataFreshness()
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

void InfoManager::updateExistingItem(QStandardItem *item, const Report &msg)
{
    item->setData(msg.count, Count);
    item->setData(msg.time, Time);
    item->setData(msg.az[0], Az_1);
    item->setData(msg.az[1], Az_2);

    QVariant variant;
    if (m_isQint16) {
        variant.setValue(msg.info_16);
    } else {
        variant.setValue(msg.info_8);
    }
    item->setData(variant, Info);
}

void InfoManager::setData(QStandardItem *item, const Report &msg)
{
    item->setData(msg.channel, ChannelNumber);
    item->setData(msg.kaNumber, KaNumber);
    item->setData(msg.acState, AcState);
    updateExistingItem(item, msg);
}
