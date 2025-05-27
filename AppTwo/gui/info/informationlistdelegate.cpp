#include "informationlistdelegate.h"

#include <QPainter>

InformationListDelegate::InformationListDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

void InformationListDelegate::paint(QPainter *painter,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
{
    Q_UNUSED(index)

    painter->fillRect(option.rect, "#3C388D");

    QPen pen;
    pen.setWidth(2);
    pen.setColor("#9FC53A");
    painter->setPen(pen);

    painter->drawLine(5, 40, option.rect.right(), 40);

    pen.setColor("white");
    painter->setPen(pen);

    QFont font;
    font.setBold(true);
    font.setWeight(QFont::DemiBold);
    painter->setFont(font);

    painter->drawText(option.rect.adjusted(20, 10, 0, 0), "Канал данных");

    font.setBold(false);
    painter->setFont(font);

    painter->drawText(25, 60, "Номер КА");
    painter->drawText(25, 80, "Координаты");
    painter->drawText(25, 100, "Состояние инфраструктуры АС");
    painter->drawText(25, 130, "Время привязки первого отсчета");

    painter->save();

    painter->restore();
}

QSize InformationListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(150, 150);
}
