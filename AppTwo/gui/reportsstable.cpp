#include "reportsstable.h"

#include "reportsmodel.h"

using namespace View;

ReportsTable::ReportsTable(QWidget *parent)
    : QWidget(parent)
    , m_tableView(new QTableView(this))
    , m_model(new ReportsModel(this))
{
    setFixedSize(300, 150);
    setStyleSheet("background-color: #E4E5FF");

    m_tableView->setModel(m_model);
    m_tableView->setColumnWidth(0, 110);
    m_tableView->setColumnWidth(1, 110);
    m_tableView->setFixedSize(240, 150);

    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
}

void ReportsTable::clear()
{
    m_model->clear();
}

void ReportsTable::append(short azimuth, short elevation)
{
    m_model->append(azimuth, elevation);
}
