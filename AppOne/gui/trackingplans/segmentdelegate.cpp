#include "segmentdelegate.h"

#include "segmentslist.h"

#include <QEvent>
#include <QPainter>
#include <QMouseEvent>

using namespace View;

SegmentDelegate::SegmentDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

QSize SegmentDelegate::sizeHint(const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(280, 180);
}
void SegmentDelegate::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    painter->save();
    painter->fillRect(option.rect, "#B2EEBB");

    QFont font;
    font.setPixelSize(14);
    painter->setFont(font);

    auto a = index.data(SegmentsList::SectorNumber);

    auto endTime = index.data(SegmentsList::EndTime).toString();
    auto startTime = index.data(SegmentsList::StartTime).toString();
    auto targetCount = index.data(SegmentsList::TargetCount).toString();
    auto sectorNumber = index.data(SegmentsList::SectorNumber).toString();
    auto centerFrequency = index.data(SegmentsList::CenterFrequency).toString();
    auto spacecraftNumber = index.data(SegmentsList::SpacecraftNumber).toString();
    auto physicalChannelNumber = index.data(SegmentsList::PhysicalChannelNumber).toString();
    auto polarizationDirection = index.data(SegmentsList::PolarizationDirection).toString();

    painter->drawText(10, 20, "№ КА: " + spacecraftNumber);
    painter->drawText(10, 40, "№ физ. канала: " + physicalChannelNumber);
    painter->drawText(10, 60, "№ сектора приема: " + sectorNumber);

    painter->drawText(10, 90, "Частота: \n" + centerFrequency);
    painter->drawText(10, 120, "Направление поляризации: " + polarizationDirection);
    painter->drawText(10, 150, "Временной промежуток отрезков:" + startTime + " - " + endTime);

    painter->restore();
}
