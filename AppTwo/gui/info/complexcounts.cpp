#include "complexcounts.h"

using namespace View;

ComplexCounts::ComplexCounts(QWidget *parent)
    : QListView(parent)
    , m_model(new QStandardItemModel(this))
{
    setFixedSize(270, 350);
}

void ComplexCounts::addCount()
{
    auto *item = new QStandardItem();
    m_model->appendRow(item);
}
