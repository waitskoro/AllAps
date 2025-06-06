#include "channeldatadelegate.h"

#include "channeldatalist.h"

#include <QPainter>
#include <QMouseEvent>

using namespace View;

ChannelDataDelegate::ChannelDataDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

void ChannelDataDelegate::paint(QPainter *painter,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    Q_UNUSED(index)

    QFont font;
    font.setPixelSize(14);
    font.setBold(false);
    painter->setFont(font);

    painter->save();

    if (index == m_clickedIndex) painter->fillRect(option.rect, "#4640C0");
    else painter->fillRect(option.rect, "#3C388D");

    QPen pen;
    pen.setColor("white");
    painter->setPen(pen);

    font.setBold(true);
    font.setWeight(QFont::Medium);
    painter->setFont(font);

    painter->drawText(option.rect.x() + 10,
                      option.rect.y() + 20,
                      QString("Канал данных №%1")
                          .arg(index.data(ChannelDataList::ChannelNumber).toInt()));

    QRect textRect = option.rect.adjusted(10, 35, 80, 35);
    painter->drawText(textRect,
                      Qt::TextWordWrap,
                      QString("Количество отрезков\nзапланированного сеанса\nслежения -  %1")
                          .arg(index.data(ChannelDataList::SegmentCount).toString()));

    pen.setColor("#9FC53A");
    painter->setPen(pen);
    painter->drawLine(0, 30, option.rect.right(), 30);

    painter->restore();
}

QSize ChannelDataDelegate::sizeHint(const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(235, 100);
}

bool ChannelDataDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            m_clickedIndex = index;

            emit const_cast<QAbstractItemModel*>(index.model())->dataChanged(index, index);

            emit itemClicked(index.data(ChannelDataList::ChannelNumber).toInt());
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
