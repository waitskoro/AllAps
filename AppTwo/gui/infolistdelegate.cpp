#include "infolistdelegate.h"

#include "common/enums.h"

#include <QEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QModelIndex>

InfoListDelegate::InfoListDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

void InfoListDelegate::paint(QPainter *painter,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    Q_UNUSED(index)
    Q_UNUSED(option)

    if (index == m_clickedIndex) {
        painter->fillRect(option.rect, "#2F7689");
        painter->setPen("white");
    } else {
        painter->fillRect(option.rect, "#F4F4F4");
        painter->setPen("black");
    }

    auto top = option.rect.top();
    auto left = option.rect.left();
    auto right = option.rect.right();
    auto bottom = option.rect.bottom();

    painter->drawText(left + 80, top + 20, "КА: " + index.data(KaNumber).toString());
    painter->drawText(left + 130, top + 20, "Канал данных: " + index.data(ChannelNumber).toString());

    painter->drawText(QRect(left + 15, top + 40, 150, 90),
                      Qt::TextWordWrap,
                      "Состояние \nинфраструктуры АС:");

    painter->drawText(QRect(left + 170, top + 30, 230, 90),
                      Qt::TextWordWrap,
                      "Обнаружена неисправность,  ухудшающая характеристики приема");

    painter->drawLine(left + 10, top + 25,
                      right - 10, top + 25);

    painter->drawLine(left + 10, bottom - 50,
                      right - 10, bottom - 50);

    painter->drawText(QRect(left + 15, bottom - 45, 150, 90),
                      Qt::TextWordWrap,
                      "Время привязки \nпервого отсчета: ");

    painter->drawText(QRect(left + 170, top + 105, 150, 25),
                      "10.02.2025 19:23:09");

    painter->save();
    painter->restore();
}

bool InfoListDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                   const QStyleOptionViewItem &option,
                                   const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            m_clickedIndex = index;

            emit const_cast<QAbstractItemModel*>(index.model())->dataChanged(index, index);

            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QSize InfoListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(280, 140);
}
