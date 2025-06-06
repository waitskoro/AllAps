#include "infolistdelegate.h"

#include "common/enums.h"

#include <QEvent>
#include <QPainter>
#include <QDateTime>
#include <QMouseEvent>
#include <QModelIndex>

InfoListDelegate::InfoListDelegate(QWidget *parent)
    : QStyledItemDelegate(parent)
    , m_label(new QLabel(parent))
{
    m_label->setText("  !");
    m_label->setFixedSize(17, 15);
    m_label->setStyleSheet("background-color: red;");
}

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
    auto right = option.rect.right();

    painter->drawText(left + 100, top + 20, "КА: " + index.data(KaNumber).toString());
    painter->drawText(left + 150, top + 20, "Канал данных: " + index.data(ChannelNumber).toString());

    quint8 acState = static_cast<quint8>(index.data(AcState).toUInt());

    QString stateMessage;
    QStringList errors;

    if (acState == 0) {
        stateMessage = "Данные пригодны";
    } else {
        stateMessage = "Данные не пригодны";

        if (acState & 0x01) {
            errors << "Время не синхронизировано с UTC";
        }
        if (acState & 0x02) {
            errors << "Обнаружена неисправность (без ухудшения характеристик приёма)";
        }
        if (acState & 0x08) {
            errors << "Обнаружена неисправность (с ухудшением характеристик приёма)";
        }
        if ((acState & 0xF4) != 0) {
            errors << "Неизвестный статус оборудования";
        }
    }

    painter->drawLine(left + 10, top + 30,
                      right - 10, top + 30);

    painter->drawText(left + 20, top + 50,
                      "Состояние АС:");

    painter->drawText(left + 170, top + 50,
                      stateMessage);

    painter->drawText(QRect(left + 20, top + 60, 150, 90),
                      "Время привязки первого отсчета: ");

    double data = index.data(Time).toDouble();
    QDateTime dateTime = fromDoubleToDate(data);

    painter->drawText(QRect(left + 170, top + 70, 150, 25),
                      dateTime.toString("dd.MM.yy hh:mm:ss"));

    painter->save();
    painter->restore();

    m_label->move(option.rect.right() - 20, option.rect.top() + 10);
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
    return QSize(280, 100);
}
