#include "stateworkinglist.h"

#include "stateworkingdelegate.h"

#include <QPainter>

using namespace View;

StateWorkingList::StateWorkingList(QWidget *parent)
    : QListView(parent)
    , m_model(new QStandardItemModel(this))
{
    auto *delegate = new StateWorkingDelegate(this);
    setItemDelegate(delegate);

    setEditTriggers(QAbstractItemView::NoEditTriggers);

    setSpacing(5);

    setFixedSize(370, 240);
    setModel(m_model.get());
    setStyleSheet("background-color: white");

    connect(delegate,
            &StateWorkingDelegate::itemClicked,
            this,
            &StateWorkingList::itemClicked);
}

void StateWorkingList::clear()
{
    m_model->clear();
}

void StateWorkingList::addState(CdoMessage &state, int sector)
{
    auto *item = new QStandardItem();
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    item->setData(state.state, State);
    item->setData(state.amperage, Amperage);
    item->setData(state.voltage, Voltage);
    item->setData(state.temperature, Temperature);
    item->setData(state.cdoState, CdoState);
    item->setData(state.camCount, CamCount);
    item->setData(sector, SectorNumber);

    QVariant variant;
    variant.setValue(state.camInfo);
    item->setData(variant, CamInfo);

    m_model->appendRow(item);
}
