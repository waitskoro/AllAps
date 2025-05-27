#pragma once

#include <QObject>
#include <QListView>
#include <QStandardItemModel>

namespace View {

class ComplexCounts : public QListView
{
    Q_OBJECT
public:
    explicit ComplexCounts(QWidget *parent = nullptr);

    void addCount();

private:
    QScopedPointer<QStandardItemModel> m_model;
};

}
