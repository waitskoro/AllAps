#include "statecamdelegate.h"

#include "statecamlist.h"
#include "statecamradiators.h"

#include <QEvent>
#include <QPainter>
#include <QMouseEvent>

using namespace View;

StateCamDelegate::StateCamDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

QSize StateCamDelegate::sizeHint(const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(220, 100);
}

void StateCamDelegate::paint(QPainter *painter,
                             const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    painter->save();

    painter->fillRect(option.rect, "#36CCE8");

    painter->drawLine(option.rect.right() - 30, option.rect.top(),
                      option.rect.right() - 30, option.rect.bottom());

    QPolygonF triangle;
    int triangleSize = 10;
    int triangleRightMargin = 7;
    int centerY = option.rect.top() + option.rect.height() / 2;

    triangle << QPointF(option.rect.right() - triangleRightMargin - triangleSize, centerY - triangleSize/2)
             << QPointF(option.rect.right() - triangleRightMargin, centerY)
             << QPointF(option.rect.right() - triangleRightMargin - triangleSize, centerY + triangleSize/2);

    painter->setBrush(Qt::black);
    painter->setPen(Qt::black);
    painter->drawPolygon(triangle);

    auto state =  index.data(StateCamList::State).toString();
    auto amperage =  QString("Ток потребления: %1A").arg(index.data(StateCamList::Amperage).toString());
    auto temperature =  QString("Температура: %1°").arg(index.data(StateCamList::Temperature).toString());

    if (state == "0") {
        state = "в норме";
    } else if (state == "1") {
        state = "не подается опорная частота 10 МГц";
    } else if (state == "2") {
        state = "не подается сигнал 1 сек";
    } else if (state == "3") {
        state = "нет синхронизации с опорной частотой";
    } else if (state == "4") {
        state = "сбой при передаче комплексных отсчетов";
    } else if (state == "5") {
        state = "амплитудное распределение вне нормы";
    } else if (state == "6") {
        state = "фазовое распределение вне нормы;";
    } else if (state == "7") {
        state = "прием отсутствует";
    } else if (state == "8") {
        state = "присутствует помеха в полосе 1543-1546 МГц";
    } else if (state == "255") {
        state = "недоступен";
    }

    painter->drawText(QRect(option.rect.left() + 10, option.rect.top() + 5, 220, 40),
                      Qt::TextWordWrap | Qt::AlignVCenter,
                      QString("Состояние:   %1").arg(state));

    painter->drawText(option.rect.left() + 10, option.rect.top() + 60, amperage);
    painter->drawText(option.rect.left() + 10, option.rect.top() + 80, temperature);

    painter->restore();
}

bool StateCamDelegate::editorEvent(QEvent *event,
                                   QAbstractItemModel *model,
                                   const QStyleOptionViewItem &option,
                                   const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QRect buttonRect(option.rect.right() - 30, option.rect.top(), 30, option.rect.height());

        if (buttonRect.contains(mouseEvent->pos())) {
            StateCamRadiators *stateCamRadiators = new StateCamRadiators();
            stateCamRadiators->move(725, option.rect.top() + 315);
            stateCamRadiators->show();

            QVariant variant = index.data(StateCamList::EmmState);
            qint32 count = index.data(StateCamList::EmmCount).toInt();
            stateCamRadiators->setInfo(count, variant);
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
