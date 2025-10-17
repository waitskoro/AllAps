#pragma once

#include <QTimer>
#include <QObject>
#include <QListView>
#include <QStandardItemModel>

#include "src/messagesinfo.h"

namespace View {

class PlansList : public QListView
{
    Q_OBJECT

public:
    enum Roles {
        CenterFrequency  = Qt::UserRole + 1,
        SpacecraftNumber,
        CurrentAzimut,
        CurrentAngle,
        ChannelNumber,
        DirectionOfPolarizaion,
        LevelOfSignal,
        ReceivingSectorNumber,
        State,
        AzimutStartSector,
        AzimutEndSector,
    };

    explicit PlansList(QWidget *parent = nullptr);
    ~PlansList();
    void clear();
    void addMessage(const ReceivingMessage &);

signals:
    void messagesIsEmpty();

private:
    bool isNew;

    bool m_isDestroying = false;

    std::atomic<bool> m_isActive{true};
    QScopedPointer<QTimer> m_clearTimer;
    QScopedPointer<QTimer> m_refreshTimer;
    QScopedPointer<QStandardItemModel> m_model;
};

}
