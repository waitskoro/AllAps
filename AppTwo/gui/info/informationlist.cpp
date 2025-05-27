#include "informationlist.h"

#include "informationlistdelegate.h"

using namespace View;

InformationList::InformationList(QWidget *parent)
    : QListView(parent)
    , m_model(new QStandardItemModel(this))
{
    auto *delegate = new InformationListDelegate(this);
    setItemDelegate(delegate);

    setFixedSize(350, 350);

    setSpacing(5);
    setModel(m_model.get());
}

void InformationList::addInfo()
{
    auto *item = new QStandardItem();
    m_model->appendRow(item);
}
