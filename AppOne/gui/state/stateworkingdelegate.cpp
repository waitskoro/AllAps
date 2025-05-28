#include "stateworkingdelegate.h"

#include <QPainter>

using namespace View;

StateWorkingDelegate::StateWorkingDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

QSize StateWorkingDelegate::sizeHint(const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(450, 100);
}

void StateWorkingDelegate::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    Q_UNUSED(index)

    painter->fillRect(option.rect, QColor(0x6E, 0x87, 0xD8));

    painter->save();
    painter->restore();
}
