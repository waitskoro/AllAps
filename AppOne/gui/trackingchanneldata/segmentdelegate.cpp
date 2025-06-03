#include "segmentdelegate.h"

#include "segmentslist.h"
#include "src/contants.h"

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
    return QSize(280, 130);
}
void SegmentDelegate::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    painter->save();
    painter->fillRect(option.rect, "#BDE5ED");

    painter->fillRect(QRect(option.rect.left(), 0,
                            option.rect.width(), 30), "#3C388D");

    QFont font;
    font.setPixelSize(14);
    painter->setFont(font);

    QPen pen;
    pen.setColor("white");
    painter->setPen(pen);

    auto endTime = index.data(SegmentsList::EndTime);
    auto startTime = index.data(SegmentsList::StartTime);
    auto targetCount = index.data(SegmentsList::TargetCount).toString();
    auto sectorNumber = index.data(SegmentsList::SectorNumber).toString();
    auto centerFrequency = index.data(SegmentsList::CenterFrequency).toString();
    auto spacecraftNumber = index.data(SegmentsList::SpacecraftNumber).toString();
    auto physicalChannelNumber = index.data(SegmentsList::PhysicalChannelNumber).toString();
    auto polarizationDirection = index.data(SegmentsList::PolarizationDirection);

    painter->drawText(30, 20, "№ КА: " + spacecraftNumber);
    painter->drawText(110, 20, "№ физ. канала: " + physicalChannelNumber);
    painter->drawText(270, 20, "№ сектора: " + sectorNumber);

    pen.setColor("black");
    painter->setPen(pen);

    painter->setFont(font);

    QDateTime startDate = fromDoubleToDate(startTime.toDouble());
    QDateTime endDate = fromDoubleToDate(endTime.toDouble());

    auto startTimeS = startDate.toString("dd.MM.yy hh:mm");
    auto endTimeS =  endDate.toString("dd.MM.yy hh:mm");

    auto freq = QString("Частота: %1 кГц").arg(centerFrequency);

    QString polS;

    if (polarizationDirection == 0) {
        polS = "правая круговая";
    } else if (polarizationDirection == 1) {
        polS = "левая круговая";
    } else if (polarizationDirection == 2) {
        polS = "вертикальная";
    } else if (polarizationDirection == 3) {
        polS = "горизонтальная";
    } else if (polarizationDirection == 4) {
        polS = "линейная +45°";
    } else if (polarizationDirection == 5) {
        polS = "линейная –45°";
    }

    auto pol = QString("Поляризации: %1").arg(polS);

    painter->drawText(10, 50, freq);

    painter->drawText(QRect(10, 60, 250, 30),
                      Qt::TextWordWrap,
                      pol);
    painter->drawText(QRect(10, 85, 250, 50),
                      Qt::TextWordWrap,
                      "Временной промежуток: \n"
                                   + startTimeS + " - " + endTimeS);

    painter->restore();
}
