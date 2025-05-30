#pragma once

#include "src/channelmessage.h"

#include <QListView>
#include <QStandardItemModel>

namespace View {

class SegmentsList : public QListView
{
    Q_OBJECT

    enum Roles {
        SectorNumber,
        PhysicalChannelNumber,
        PolarizationDirection,
        SpacecraftNumber,
        CenterFrequency,
        StartTime,
        EndTime,
        TargetCount
    };

public:
    explicit SegmentsList(QWidget *parent = nullptr);

    void clear();
    void addMessage(const DataChannelSegment &);

private:
    QScopedPointer<QStandardItemModel> m_model;
};

}
