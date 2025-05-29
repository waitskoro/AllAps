#include "statecamlist.h"

#include "statecamdelegate.h"

using namespace View;

StateCamList::StateCamList(QWidget *parent)
    : QListView(parent)
    , m_model(new QStandardItemModel(this))
{
    auto *delegate = new StateCamDelegate(this);
    setItemDelegate(delegate);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setSpacing(5);
    setFixedSize(250, 220);
    setModel(m_model.get());
    setStyleSheet("background-color: white");
}

void StateCamList::addCam(CamState &state)
{
    auto *item = new QStandardItem();
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    m_model->appendRow(item);
}

void StateCamList::clear()
{
    m_model->clear();
}
