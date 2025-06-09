#pragma once

#include <QListView>
#include <QTableView>
#include <QStandardItemModel>

namespace View {

class ReportsModel;

class ReportsTable : public QWidget
{
    Q_OBJECT
public:
    explicit ReportsTable(QWidget *parent = nullptr);

    void clear();
    void append(short azimuth, short elevation);
private:
    QTableView *m_tableView;
    ReportsModel *m_model;
};

}
