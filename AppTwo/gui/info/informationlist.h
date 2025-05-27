#pragma once

#include <QObject>
#include <QListView>
#include <QStandardItemModel>

namespace View {

class InformationList : public QListView
{
    Q_OBJECT

public:
    explicit InformationList(QWidget *parent = nullptr);

    void addInfo();

private:
    QScopedPointer<QStandardItemModel> m_model;
};

}
