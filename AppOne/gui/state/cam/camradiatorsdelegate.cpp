#include "camradiatorsdelegate.h"
#include "camradiatorslist.h"
#include <QPainter>

using namespace View;

CamRadiatorsDelegate::CamRadiatorsDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

QSize CamRadiatorsDelegate::sizeHint(const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(100, 70);
}

void CamRadiatorsDelegate::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    painter->save();
    painter->fillRect(option.rect, "#B2EEBB");

    QRect rect = option.rect;
    int yOffset = 20;

    QString number = index.data(CamRadiatorsList::Number).toString();
    QString pol = index.data(CamRadiatorsList::Pol).toString();
    QString state = index.data(CamRadiatorsList::State).toString();
    QString signalLevel = index.data(CamRadiatorsList::SignalLevel).toString();

    if (pol == "0") {
        pol = "вертикальная поляризация";
    } else if (pol == "1") {
        pol = "горизонтальная поляризация";
    }

    if (state == "0") {
        state = "в норме";
    } else if (state == "1") {
        state = "сигнал калибровки не принимается";
    } else if (state == "2") {
        state = "сигнал калибровки ниже нормы";
    } else if (state == "3") {
        state = "сигнал калибровки выше нормы";
    } else if (state == "4") {
        state = "отсутствует сигнал калибровки";
    } else if (state == "255") {
        state = "радиотракт неработоспособен";
    }

    painter->drawText(rect.left() + 10, rect.top() + yOffset, QString("Номер: %1        Сигнал: %2").arg(number).arg(signalLevel));
    yOffset += 20;
    painter->drawText(rect.left() + 10, rect.top() + yOffset, QString("Состояние: %1").arg(state));
    yOffset += 20;
    painter->drawText(rect.left() + 10, rect.top() + yOffset, QString("Назначение: %1").arg(pol));

    painter->restore();
}
