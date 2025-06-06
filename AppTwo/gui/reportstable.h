#pragma once

#include <QListView>
#include <QTableView>
#include <QStandardItemModel>

namespace View {

class ReportsTable : QWidget
{
    Q_OBJECT
public:
    explicit ReportsTable(QWidget *parent = nullptr);

private:
    QTableView *m_tableView;
    // TargetDesignationModel *m_model;
};

}
