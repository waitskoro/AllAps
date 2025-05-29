#include "stateworkingdelegate.h"

#include "stateworkinglist.h"

#include <QEvent>
#include <QPainter>
#include <QMouseEvent>

using namespace View;

StateWorkingDelegate::StateWorkingDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

QSize StateWorkingDelegate::sizeHint(const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(280, 90);
}

bool StateWorkingDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                       const QStyleOptionViewItem &option,
                                       const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            m_clickedIndex = index;

            emit const_cast<QAbstractItemModel*>(index.model())->dataChanged(index, index);

            emit itemClicked(index.row());
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
void StateWorkingDelegate::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    painter->save();

    if (index == m_clickedIndex) painter->fillRect(option.rect, "#4640C0");
    else painter->fillRect(option.rect, "#3C388D");

    QPen pen("white");
    painter->setPen(pen);

    QFont font = painter->font();
    font.setPixelSize(14);
    painter->setFont(font);

    const int leftMargin = 20;
    const int lineY = option.rect.y() + 55;
    const int col2X = 145;
    const int col3X = 275;
    const int textYOffset = option.rect.y();

    QString state48V = index.data(StateWorkingList::State).toString();
    QString stateCDO = index.data(StateWorkingList::CdoState).toString();
    QString currentCDO = index.data(StateWorkingList::Amperage).toString();
    QString voltage = index.data(StateWorkingList::Voltage).toString();
    QString temperature = index.data(StateWorkingList::Temperature).toString();

    painter->drawText(leftMargin, textYOffset + 25, "Состояние 48В:");
    painter->drawText(leftMargin + 120, textYOffset + 25, state48V);

    painter->drawText(leftMargin, textYOffset + 45, "Состояние ЦДО:");
    painter->drawText(leftMargin + 120, textYOffset + 45, stateCDO);

    painter->drawText(leftMargin + 5, textYOffset + 75, "Ток ЦДО:");
    painter->drawText(leftMargin + 80, textYOffset + 75, currentCDO);

    painter->drawText(col2X, textYOffset + 75, "Напряжение:");
    painter->drawText(col2X + 90, textYOffset + 75, voltage);

    painter->drawText(col3X, textYOffset + 75, "t°, темп.:");
    painter->drawText(col3X + 80, textYOffset + 75, temperature);

    pen.setColor("#9FC53A");
    painter->setPen(pen);
    painter->drawLine(leftMargin, lineY, option.rect.width() - leftMargin, lineY);

    pen.setColor("#E4E489");
    painter->setPen(pen);
    font.setPixelSize(12);
    painter->setFont(font);

    int rowNumber = index.row() + 1;
    painter->drawText(option.rect.right() - 40, option.rect.y() + 20,
                      "№" + QString::number(rowNumber));

    painter->restore();
}
