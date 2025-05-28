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
    int bottom = option.rect.bottom();

    painter->drawLine(left, top + 80, right, top + 80);

    painter->drawLine(left + 320, top, left + 320, top + 80);
    painter->drawLine(left + 270, top + 80, left + 270, bottom);

    QStyledItemDelegate::paint(painter, option, index);

    //----------------------------Text & Styling-----------------------------
    QFont font;
    font.setPixelSize(16);
    font.setBold(true);
    painter->setFont(font);

    QPen textPen(Qt::white);
    painter->setPen(textPen);

    drawBoldText(painter, option, index);

    font.setBold(false);
    painter->setFont(font);

    drawText(painter, option);

    painter->restore();
}

QSize PlansListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(450, 150);
}

void PlansListDelegate::drawText(QPainter *painter,
                                 const QStyleOptionViewItem &option) const
{
    //===========Номер_КА-Поляризация-Центральная частота============

    painter->drawText(option.rect.adjusted(20, 15, 0, 0), "Номер КА");
    painter->drawText(option.rect.adjusted(20, 35, 0, 0), "Поляризация");
    painter->drawText(option.rect.adjusted(20, 55, 0, 0), "Центральная частота");

    //=========================Текущее положение====================

    painter->drawText(option.rect.adjusted(330, 15, 0, 0), "Текущее положение");
    painter->drawText(option.rect.adjusted(350, 35, 0, 0), "Азимут");
    painter->drawText(option.rect.adjusted(350, 55, 0, 0), "Угол места");

    //==========Уровень_сигнала-Состояние-Диапазон-сектора==========

    painter->drawText(option.rect.adjusted(20, 85, 0, 0), "Состояние");
    painter->drawText(option.rect.adjusted(20, 105, 0, 0), "Уровень сигнала");
    painter->drawText(option.rect.adjusted(20, 125, 0, 0), "Диапазон сектора");

    //================Номер сектора-Номер физ. канала==============

    painter->drawText(option.rect.adjusted(280, 95, 0, 0), "Номер сектора");
    painter->drawText(option.rect.adjusted(280, 115, 0, 0), "Номер физ. канала");
}

void PlansListDelegate::drawBoldText(QPainter *painter,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    //===========Номер_КА-Поляризация-Центральная частота============

    QString spacecraftNumber = index.data(PlansList::SpacecraftNumber).toString();
    painter->drawText(option.rect.adjusted(205, 15, 0, 0),
                      spacecraftNumber == "" ? "0" : spacecraftNumber);

    QString polarizaion = index.data(PlansList::DirectionOfPolarizaion).toString();
    painter->drawText(option.rect.adjusted(205, 35, 0, 0),
                      polarizaion == "" ? "0" : polarizaion);

    QString centerFrequency = index.data(PlansList::CenterFrequency).toString();
    painter->drawText(option.rect.adjusted(205, 55, 0, 0),
                      centerFrequency == "" ? "0" : centerFrequency);

    //=========================Текущее положение====================

    QString currentAzimut = index.data(PlansList::CurrentAzimut).toString();
    painter->drawText(option.rect.adjusted(450, 35, 0, 0),
                      currentAzimut == "" ? "0" : currentAzimut);

    QString currentBeanAzimut = index.data(PlansList::CurrentBeanAzimut).toString();
    painter->drawText(option.rect.adjusted(450, 55, 0, 0),
                      currentBeanAzimut == "" ? "0" : currentBeanAzimut);

    //==========Уровень_сигнала-Состояние-Диапазон-сектора==========

    QString level = index.data(PlansList::LevelOfSignal).toString();
    painter->drawText(option.rect.adjusted(180, 85, 0, 0),
                      level == "" ? "0" : level);

    QString state = index.data(PlansList::State).toString();
    painter->drawText(option.rect.adjusted(180, 105, 0, 0),
                      state == "" ? "0" : state);

    QString startSector = index.data(PlansList::AzimutStartSector).toString();
    if (startSector.isEmpty()) startSector = "0";

    QString endSector = index.data(PlansList::AzimutEndSector).toString();
    if (endSector.isEmpty()) endSector = "0";

    QString sector = QString("%1° - %2°").arg(startSector, endSector);
    painter->drawText(option.rect.adjusted(180, 125, 0, 0), sector);

    //================Номер сектора-Номер физ. канала==============

    QString numberSector = index.data(PlansList::ReceivingSectorNumber).toString();
    painter->drawText(option.rect.adjusted(450, 95, 0, 0),
                      numberSector == "" ? "0" : numberSector);

    QString numberChannel = index.data(PlansList::ChannelNumber).toString();
    painter->drawText(option.rect.adjusted(450, 115, 0, 0),
                      numberChannel == "" ? "0" : numberChannel);
}
