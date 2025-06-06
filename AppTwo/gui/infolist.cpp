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

    setFixedSize(400, 420);

    setSpacing(5);
    setModel(m_model.get());

    setStyleSheet("background-color: #ACB8BF");
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

    QVariant variant;
    variant.setValue(msg.info);
    item->setData(variant, Info);

    m_model->appendRow(item);
}
