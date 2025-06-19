#include "statecamlist.h"

#include "statecamdelegate.h"

using namespace View;

StateCamList::StateCamList(QWidget *parent)
    : QListView(parent)
    , m_model(new QStandardItemModel(this))
{
    auto *delegate = new StateCamDelegate(this);
    setItemDelegate(delegate);

    setSpacing(5);
    setFixedSize(290, 220);
    setModel(m_model.get());
    setStyleSheet("background-color: white");
}

void StateCamList::addCam(CamState &state)
{
    auto *item = new QStandardItem();
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    item->setData(state.camState, State);
    item->setData(state.amperage, Amperage);
    item->setData(state.temperature, Temperature);

    item->setData(state.camCount, EmmCount);

    QVariant variant;
    variant.setValue(state.emmState);
    item->setData(variant, EmmState);

    m_model->appendRow(item);
}

void StateCamList::clear()
{
    m_model->clear();
}
