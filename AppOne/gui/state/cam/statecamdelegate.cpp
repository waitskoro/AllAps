#include "statecamdelegate.h"

#include <QPainter>

using namespace View;

StateCamDelegate::StateCamDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

QSize StateCamDelegate::sizeHint(const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(220, 90);
}

void StateCamDelegate::paint(QPainter *painter,
                             const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    painter->save();

    painter->fillRect(option.rect, "#36CCE8");

    int rowNumber = index.row() + 1;
    painter->drawText(option.rect.right() - 30, option.rect.y() + 20,
                      "№" + QString::number(rowNumber));

    painter->restore();
}
