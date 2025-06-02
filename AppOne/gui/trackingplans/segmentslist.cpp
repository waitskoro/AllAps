#include "segmentslist.h"

#include "segmentdelegate.h"

using namespace View;

SegmentsList::SegmentsList(QWidget *parent)
    : QListView(parent)
    , m_model(new QStandardItemModel(this))
{
    auto *delegate = new SegmentDelegate(this);
    setItemDelegate(delegate);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setSpacing(3);
    setFixedSize(395, 400);
    setModel(m_model.get());
    setStyleSheet("background-color: #EBEBEB");
}

void SegmentsList::clear()
{
    m_model->clear();
}

void SegmentsList::addMessage(const DataChannelSegment &msg)
{
    auto *item = new QStandardItem();
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    item->setData(msg.physicalChannelNumber, PhysicalChannelNumber);
    item->setData(msg.sectorNumber, SectorNumber);
    item->setData(msg.polarizationDirection, PolarizationDirection);
    item->setData(msg.spacecraftNumber, SpacecraftNumber);
    item->setData(msg.centerFrequency, CenterFrequency);
    item->setData(msg.startTime, StartTime);
    item->setData(msg.endTime, EndTime);
    item->setData(msg.targetCount, TargetCount);

    qDebug() << msg.sectorNumber << "sector number in segment list";

    m_model->appendRow(item);
}
