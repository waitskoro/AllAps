#include "infolist.h"

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

void InformationList::addInfo()
{
    auto *item = new QStandardItem();
    m_model->appendRow(item);
}
