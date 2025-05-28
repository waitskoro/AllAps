#include "channeldatadelegate.h"
#include <QPainter>
#include <QMouseEvent>

using namespace View;

ChannelDataDelegate::ChannelDataDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
    , m_buttonView(new QRect(0, 0, 150, 30))
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
    painter->fillRect(option.rect, QColor(0x6E, 0x87, 0xD8));

    m_buttonView->moveTo(option.rect.right() - 170, option.rect.y() + 20);
    m_currentButtonRect = *m_buttonView;

    painter->fillRect(*m_buttonView, Qt::white);

    painter->drawText(option.rect.x() + 30, option.rect.y() + 25,
                     "Номер канала данных");

    QRect textRect = option.rect.adjusted(30, 35, 80, 35);
    painter->drawText(textRect, Qt::TextWordWrap,
                     "Количество отрезков\nзапланированного сеанса слежения");

    painter->drawText(*m_buttonView, Qt::AlignCenter, "Посмотреть");

    painter->restore();
}

QSize ChannelDataDelegate::sizeHint(const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(450, 80);
}

bool ChannelDataDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

        if (m_currentButtonRect.contains(mouseEvent->pos())) {
            emit buttonClicked(index);
            qDebug() << "clicked";
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
