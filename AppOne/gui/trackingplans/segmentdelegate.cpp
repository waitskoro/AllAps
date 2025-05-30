#include "segmentdelegate.h"

// #include "segmentslist.h"

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
    return QSize(280, 90);
}
void SegmentDelegate::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    painter->save();
    painter->fillRect(option.rect, "#B2EEBB");

    int rowNumber = index.row() + 1;
    painter->drawText(option.rect.right() - 40, option.rect.y() + 20,
                      "№" + QString::number(rowNumber));

    painter->restore();
}
