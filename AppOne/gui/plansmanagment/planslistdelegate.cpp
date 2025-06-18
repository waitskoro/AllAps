#include "planslistdelegate.h"

#include <QPainter>

#include "planslist.h"

using namespace View;

PlansListDelegate::PlansListDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

void PlansListDelegate::paint(QPainter *painter,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
{
    painter->save();
    painter->fillRect(option.rect, "#2C4B65");

    //----------------------------Lines------------------------------
    QPen linePen(Qt::white);
    linePen.setWidth(1);
    painter->setPen(linePen);

    int left = option.rect.left();
    int right = option.rect.right();
    int top = option.rect.top();

    painter->drawLine(left, top + 30, right, top + 30);

    QStyledItemDelegate::paint(painter, option, index);

    //----------------------------Text & Styling-----------------------------
    QFont font;
    font.setPixelSize(16);
    font.setBold(true);
    painter->setFont(font);

    QPen textPen(Qt::white);
    painter->setPen(textPen);

    //---------------------------------

    auto spacecraft = index.data(PlansList::SpacecraftNumber).toString();
    auto phyNumber = index.data(PlansList::ChannelNumber).toString();
    QString mainText = QString("КА: %1    |    № Физ. канала: %2").
                       arg(spacecraft).
                       arg(phyNumber);

    painter->drawText(QRect(0, top + 2, option.rect.width(), 30),
                      Qt::AlignCenter,
                      mainText);

    //---------------------------------

    font.setBold(false);
    painter->setFont(font);

    auto sectorNumber = index.data(PlansList::ReceivingSectorNumber).toString();

    auto startSector = index.data(PlansList::AzimutStartSector).toString();
    auto endSector = index.data(PlansList::AzimutEndSector).toString();

    QRect roundedRect(QRect(left + 10, top + 40, 180, 50));
    painter->setBrush(QBrush("#233E55"));
    painter->setPen("#233E55");
    painter->drawRoundedRect(roundedRect, 10, 10);

    painter->setBrush(QBrush("white"));
    painter->setPen("white");

    painter->drawText(QRect(left + 20, top + 45, roundedRect.width(), 50),
                      "Сектор: " + sectorNumber);

    painter->drawText(QRect(left + 20, top + 65, roundedRect.width(), 50),
                      QString("Диапазон: %1° - %2°").arg(startSector).arg(endSector));

    //---------------------------------

    painter->setPen("white");
    painter->setFont(font);

    auto freq = index.data(PlansList::CenterFrequency).toString();
    auto signal = index.data(PlansList::LevelOfSignal).toString();

    auto azimut = index.data(PlansList::CurrentAzimut).toString();
    auto azimutBean = index.data(PlansList::CurrentBeanAzimut).toString();

    painter->drawText(QRect(left + 230, top + 40, 330, 25),
                      QString("Азимут: %1°      Угол места: %2°").arg(azimut).arg(azimutBean));
    painter->drawText(QRect(left + 230, top + 65, 330, 25),
                      QString("Частота: %1 МГц      Сигнал: %2 мкВ").arg(freq).arg(signal));

    painter->restore();

    //---------------------------------

    auto polarizaion = index.data(PlansList::DirectionOfPolarizaion).toString();

    if (polarizaion == "0") {
        polarizaion = "правая круговая";
    } else if (polarizaion == "1") {
        polarizaion = "левая круговая";
    } else if (polarizaion == "2") {
        polarizaion = "вертикальная";
    } else if (polarizaion == "3") {
        polarizaion = "горизонтальная";
    } else if (polarizaion == "4") {
        polarizaion = "линейная +45°";
    } else if (polarizaion == "5") {
        polarizaion = "линейная –45°";
    }

    painter->setPen("white");
    painter->setFont(font);

    painter->drawText(QRect(left + 20, top + 95, 150, 50),
                      QString("Поляризация: %1").arg(polarizaion));

    auto state = index.data(PlansList::DirectionOfPolarizaion).toString();

    if (state != "0") {
        font.setPixelSize(14);
        painter->setFont(font);
    }


    if (state == "0") {
        state = "в норме";
    } else if (state == "1") {
        state = "были самовосстанавливающиеся отказы (сбои), сбрасывается после выдачи сообщения";
    } else if (state == "2") {
        state = "имеются некритичные отказы, не приводят к существенным ухудшениям характеристик";
    } else if (state == "3") {
        state = "имеются критичные отказы, приводят к существенным ухудшениям характеристик";
    } else if (state == "255") {
        state = "полный отказ, невозможность функционировать по  назначению";
    }

    painter->drawText(QRect(left + 180, top + 90, 400, 50),
                      Qt::AlignVCenter | Qt::TextWordWrap,
                      QString("Состояние: %1").arg(state));
}

QSize PlansListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(450, 150);
}


bool PlansListDelegate::editorEvent(QEvent *,
                                    QAbstractItemModel *,
                                    const QStyleOptionViewItem &,
                                    const QModelIndex &)
{
    return true;
}
