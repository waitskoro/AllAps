#include "reportstable.h"

using namespace View;

ReportsTable::ReportsTable(QWidget *parent)
    : QWidget(parent)
    , m_tableView(new QTableView(this))
{
    setFixedSize(300, 150);
    setStyleSheet("background-color: #E4E5FF");

    // m_tableView->setModel(m_model);
    m_tableView->setColumnWidth(0, 110);
    m_tableView->setColumnWidth(1, 110);
    m_tableView->setFixedSize(240, 150);

    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
}
