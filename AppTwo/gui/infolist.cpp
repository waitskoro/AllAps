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

    setFixedSize(660, 420);

    setSpacing(5);
    setModel(m_model.get());

    setStyleSheet("background-color: #E4E5FF");

    Report report;
    addInfo(report);
    addInfo(report);
    addInfo(report);
}

void InformationList::addInfo(Report msg)
{
    auto *item = new QStandardItem();
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

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

    m_model->appendRow(item);
}
