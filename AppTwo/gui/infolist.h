#pragma once

#include "common/messages.h"
#include "infolistdelegate.h"

#include <QTimer>
#include <QObject>
#include <QListView>
#include <QStandardItemModel>

namespace View {

class InformationList : public QListView
{
    Q_OBJECT

public:
    explicit InformationList(QWidget *parent = nullptr);

    void addInfo(const Report &msg);

private:
    bool isNew;

    void setData(QStandardItem *item, const Report &msg);

    QScopedPointer<QTimer> m_clearTimer;
    QScopedPointer<QTimer> m_refreshTimer;
    QScopedPointer<QStandardItemModel> m_model;

    InfoListDelegate *m_delegate;

};

}
