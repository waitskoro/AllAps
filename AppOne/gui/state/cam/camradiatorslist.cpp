#include "camradiatorslist.h"

#include "camradiatorsdelegate.h"

using namespace View;

CamRadiatorsList::CamRadiatorsList(QWidget *parent)
    : QListView{parent}
    , m_model(new QStandardItemModel(this))
{
    auto *delegate = new CamRadiatorsDelegate(this);
    setItemDelegate(delegate);

    setSpacing(5);
    setModel(m_model.get());
    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void CamRadiatorsList::clear()
{
    m_model->clear();
}

void CamRadiatorsList::addRadiators(EmmiterState *state)
{
    auto *item = new QStandardItem();
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    item->setData(state->number, Number);
    item->setData(state->pol, Pol);
    item->setData(state->state, State);
    item->setData(state->signalLevel, SignalLevel);

    m_model->appendRow(item);
}
