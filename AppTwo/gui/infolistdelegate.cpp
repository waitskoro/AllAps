#include "infolistdelegate.h"

#include "common/enums.h"

#include <QEvent>
#include <QPainter>
#include <QDateTime>
#include <QMouseEvent>
#include <QModelIndex>

InfoListDelegate::InfoListDelegate(QWidget *parent)
    : QStyledItemDelegate(parent)
{}

QDateTime InfoListDelegate::fromDoubleToDate(double dateValue) const
{
    const double unixTime = (dateValue - 25569) * 86400;
    return QDateTime::fromSecsSinceEpoch(static_cast<qint64>(unixTime));
}

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
        painter->fillRect(option.rect, QColor(54, 204, 232, 50));
        painter->setPen("black");
    }

    auto top = option.rect.top();
    auto left = option.rect.left();

    QFont font;
    font.setBold(true);
    font.setPixelSize(16);
    font.setWeight(QFont::DemiBold);
    painter->setFont(font);

    painter->drawText(left + 230, top + 20, "КА: " + index.data(KaNumber).toString());
    painter->drawText(left + 280, top + 20, "Канал данных: " + index.data(ChannelNumber).toString());

    painter->drawText(left + 20, top + 45,
                      "Состояние инфраструктуры");

    painter->drawText(left + 340, top + 50,
                      "Азимут:");
    painter->drawText(left + 470, top + 50,
                      "Угол места:");

    painter->drawText(QRect(left + 340, top + 60, 150, 50),
                      "Время привязки первого отсчета: ");

    font.setBold(false);
    painter->setFont(font);

    quint8 acState = static_cast<quint8>(index.data(AcState).toUInt());

    bool timeError = false;
    bool toBadError = false;
    bool badError = false;

    if (acState != 0) {
        if (acState & 0x01) {
            timeError = true;
        }
        if (acState & 0x02) {
            badError = true;
        }
        if (acState & 0x08) {
            toBadError = true;
        }
    }

    painter->drawText(left + 410, top + 50, index.data(Az_1).toString());
    painter->drawText(left + 570, top + 50, index.data(Az_2).toString());

    double data = index.data(Time).toDouble();
    QDateTime dateTime = fromDoubleToDate(data);

    painter->drawText(left + 480, top + 95,
                      dateTime.toString("dd.MM.yy hh:mm:ss"));

    font.setPixelSize(14);
    painter->setFont(font);

    painter->drawText(left + 40, top + 65,
                      QString("Время синхронизировано с UTC:  %1").arg(timeError ? "Нет" : "Да"));
    painter->drawText(left + 40, top + 85,
                      QString("Неисправность (с ухудшением):  %1").arg(toBadError ? "Да" : "Нет"));
    painter->drawText(left + 40, top + 105,
                      QString("Неисправность (без ухудшения):  %1").arg(badError ? "Да" : "Нет"));

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
    return QSize(280, 120);
}
