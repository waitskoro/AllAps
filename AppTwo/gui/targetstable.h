#pragma once

#include <QListView>
#include <QTableView>
#include <QStandardItemModel>

namespace View {

class TargetDesignationModel;

class TargetTable : public QWidget
{
    Q_OBJECT
public:
    explicit TargetTable(QWidget *parent = nullptr);

private:
    QTableView *m_tableView;
    TargetDesignationModel *m_model;
};

}
