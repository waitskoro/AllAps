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
    return QSize(260, 130);
}

bool StateWorkingDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                       const QStyleOptionViewItem &option,
                                       const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            m_clickedIndex = index;

            std::vector<CamState> values;

            QVariant variant = index.data(StateWorkingList::CamInfo);

            auto camCount = index.data(StateWorkingList::CamCount);
            CamState* vectorData = variant.value<CamState*>();

            for (int i = 0; i < camCount.toInt(); i++) {
                values.push_back(vectorData[i]);
            }

            emit itemClicked(values, index.data(StateWorkingList::SectorNumber).toInt());
            emit const_cast<QAbstractItemModel*>(index.model())->dataChanged(index, index);

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
    const int lineY = option.rect.y() + 70;
    const int col2X = 145;
    const int col3X = 275;
    const int textYOffset = option.rect.y();

    QString state48V = index.data(StateWorkingList::State).toString();
    QString stateCDO = index.data(StateWorkingList::CdoState).toString();
    QString currentCDO = index.data(StateWorkingList::Amperage).toString();
    QString voltage = index.data(StateWorkingList::Voltage).toString();
    QString temperature = index.data(StateWorkingList::Temperature).toString();

    painter->drawText(leftMargin, textYOffset + 20, "Состояния:");

    painter->drawText(leftMargin, textYOffset + 40, "48В:");
    painter->drawText(leftMargin + 35, textYOffset + 40, /*state48V*/"Некритичный отказ преобразователей");

    painter->drawText(leftMargin, textYOffset + 60, "ЦДО:");
    painter->drawText(leftMargin + 40, textYOffset + 60, /*stateCDO*/"Cбой при передаче комплексных отсчетов");

    painter->drawText(leftMargin, textYOffset + 90, "Ток ЦДО:");
    painter->drawText(leftMargin + 65, textYOffset + 90, /*currentCDO*/"1234");

    // painter->drawText(col2X, textYOffset + 75, "Напряжение:");
    // painter->drawText(col2X + 90, textYOffset + 75, voltage);

    // painter->drawText(col3X, textYOffset + 75, "t°, темп.:");
    // painter->drawText(col3X + 80, textYOffset + 75, temperature);

    pen.setColor("#9FC53A");
    painter->setPen(pen);
    painter->drawLine(leftMargin, lineY, option.rect.width() - leftMargin, lineY);

    painter->restore();
}
