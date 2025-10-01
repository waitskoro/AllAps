// infolist.h
#pragma once

#include <QListView>
#include <QStandardItemModel>
#include <QTimer>
#include <memory>

#include "common/messages.h"

class InfoListDelegate;

namespace View {

class InformationList : public QListView {
    Q_OBJECT
public:
    explicit InformationList(QWidget *parent = nullptr);
    void addInfo(const Report &msg);

private slots:
    void checkDataFreshness();

private:
    void setData(QStandardItem *item, const Report &msg);
    void updateExistingItem(QStandardItem *item, const Report &msg);

    std::unique_ptr<QStandardItemModel> m_model;
    InfoListDelegate *m_delegate;
    QTimer m_cleanupTimer;
    QHash<QStandardItem*, qint64> m_lastUpdateTimes;
};

} // namespace View
