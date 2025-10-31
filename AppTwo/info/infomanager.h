#pragma once

#include <QTimer>
#include <QObject>
#include <QListView>
#include <QStandardItem>

#include "src/message.h"

class InfoListDelegate;

class InfoManager : public QObject
{
    Q_OBJECT
public:
    explicit InfoManager(QObject *parent = nullptr);

    void addInfo(bool isQint16, const Report &msg);
    void setListView(QListView *listView);

private slots:
    void checkDataFreshness();

private:
    bool m_isQint16 = false;

    void setData(QStandardItem *item, const Report &msg);
    void updateExistingItem(QStandardItem *item, const Report &msg);

    QListView *m_listView;

    QTimer m_cleanupTimer;
    InfoListDelegate *m_delegate;
    std::unique_ptr<QStandardItemModel> m_model;
    QHash<QStandardItem*, qint64> m_lastUpdateTimes;
};

