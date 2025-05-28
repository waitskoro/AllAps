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

    //===========================================

    painter->fillRect(option.rect, "#C3D7E4");

    int yPos = option.rect.top() + 20;

    painter->drawText(option.rect.left() + 25, yPos, QString("КАНАЛ %1").arg("#08"));
    painter->drawText(option.rect.left() + 130, yPos, QString("️КА - %1").arg("12"));

    yPos += 5;
    painter->drawLine(option.rect.left() + 15, yPos, option.rect.right() - 15, yPos);

    yPos += 20;
    painter->drawText(option.rect.left() + 25, yPos, QString("Время: %1").arg("15.03 14:22:17.843 UTC"));

    yPos += 20;
    painter->drawText(option.rect.left() + 25, yPos, QString("️Азимут: %1°").arg("153.7"));
    painter->drawText(option.rect.left() + 160, yPos, QString("Угол места: %1°").arg("42.5"));

    yPos += 20;
    painter->drawText(option.rect.left() + 25, yPos, QString("Отсчётов: %1 (компл.)").arg("2048"));

    yPos += 10;
    painter->drawLine(option.rect.left() + 15, yPos, option.rect.right() - 15, yPos);

    yPos += 20;
    painter->drawText(option.rect.left() + 25, yPos, QString("СОСТОЯНИЕ СИСТЕМЫ:"));
    yPos += 20;
    painter->drawText(option.rect.left() + 25, yPos, QString("    Нет синхронизации с UTC"));

    //===========================================

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
