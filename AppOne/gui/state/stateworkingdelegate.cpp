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
    return QSize(260, 100);
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

            emit itemClicked(values);
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
    const int textYOffset = option.rect.y();

    QString state48V = index.data(StateWorkingList::State).toString();
    QString stateCDO = index.data(StateWorkingList::CdoState).toString();
    QString currentCDO = index.data(StateWorkingList::Amperage).toString();
    QString voltage = index.data(StateWorkingList::Voltage).toString();
    QString temperature = index.data(StateWorkingList::Temperature).toString();

    painter->drawText(leftMargin, textYOffset + 20, "Состояния:");

    if (state48V == "0") {
        state48V = "в норме";
    } else if(state48V == "1") {
        state48V = "некритичный отказ преобразователей";
    } else if(state48V == "2") {
        state48V = "критичный отказ преобразователей";
    } else if(state48V == "3") {
        state48V = "ограничение по току";
    } else if(state48V == "4") {
        state48V = "сброс по питанию";
    } else if(state48V == "255") {
        state48V = "недоступен";
    }

    painter->drawText(leftMargin + 160,
                      textYOffset + 20,
                      QString("Темп.: %1°"). arg(temperature));

    painter->drawText(leftMargin, textYOffset + 40, "48В:");
    painter->drawText(leftMargin + 35, textYOffset + 40, state48V);

    if (stateCDO == "0") {
        stateCDO = "в норме";
    } else if (stateCDO == "1") {
        stateCDO = "не подается опорная частота 10 МГц";
    } else if (stateCDO == "2") {
        stateCDO = "не подается сигнал 1 сек";
    } else if (stateCDO == "3") {
        stateCDO = "нет синхронизации с опорной частотой";
    } else if (stateCDO == "4") {
        stateCDO = "сбой при передаче комплексных отсчетов";
    } else if (stateCDO == "255") {
        stateCDO = "недоступен";
    }

    painter->drawText(leftMargin, textYOffset + 60, "ЦДО:");
    painter->drawText(leftMargin + 40, textYOffset + 60, stateCDO);

    painter->drawText(QRect(option.rect.left(), textYOffset + 70, option.rect.width(), 25),
                      Qt::AlignCenter,
                      QString("Ток потреб.: %1A     Напряжение: %2В").arg(currentCDO).arg(voltage));

    pen.setColor("#9FC53A");
    painter->setPen(pen);
    painter->drawLine(leftMargin, lineY, option.rect.width() - leftMargin, lineY);

    painter->restore();
}
