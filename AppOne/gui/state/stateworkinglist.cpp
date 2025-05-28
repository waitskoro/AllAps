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

    setSpacing(5);

    setFixedSize(600, 250);
    setModel(m_model.get());
    setStyleSheet("background-color: #C3D7E4");
}

void StateWorkingList::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
}

void StateWorkingList::addState(StateMessage &state)
{
    auto *item = new QStandardItem();
    m_model->appendRow(item);
}

