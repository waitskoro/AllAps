#pragma once

#include "src/channelmessage.h"

#include <QLabel>
#include <QListView>
#include <QStandardItemModel>

namespace View {

class SegmentsList : public QListView
{
    Q_OBJECT

public:

    enum Roles {
        SectorNumber = Qt::UserRole + 1,
        PhysicalChannelNumber,
        PolarizationDirection,
        SpacecraftNumber,
        CenterFrequency,
        StartTime,
        EndTime,
        TargetCount,
        ChannelNumber,
        Targets
    };

    explicit SegmentsList(QWidget *parent = nullptr);

    void clear();
    void addMessage(const DataChannelSegment &, qint32);

private:
    QLabel *m_headerLabel;
    QScopedPointer<QStandardItemModel> m_model;
};

}
