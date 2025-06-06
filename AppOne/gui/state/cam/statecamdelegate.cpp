#include "statecamdelegate.h"

#include "statecamlist.h"

#include <QPainter>

using namespace View;

StateCamDelegate::StateCamDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

QSize StateCamDelegate::sizeHint(const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(220, 110);
}

void StateCamDelegate::paint(QPainter *painter,
                             const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    painter->save();

    painter->fillRect(option.rect, "#36CCE8");

    auto state =  QString("Состояние: %1").arg(index.data(StateCamList::State).toString());
    auto amperage =  QString("Ток потребления ЦАМ: %1").arg(index.data(StateCamList::Amperage).toString());
    auto temperature =  QString("Температура ЦАМ: %1").arg(index.data(StateCamList::Temperature).toString());
    auto emmCount =  QString("Количество\nрадиотрактов ЦАМ: %1").arg(index.data(StateCamList::EmmCount).toString());

    painter->drawText(option.rect.left() + 10, option.rect.top() + 20, state);
    painter->drawText(option.rect.left() + 10, option.rect.top() + 40, amperage);
    painter->drawText(option.rect.left() + 10, option.rect.top() + 60, temperature);
    painter->drawText(QRect(option.rect.left() + 10, option.rect.top() + 65,
                            160, 50), Qt::TextWordWrap, emmCount);

    painter->restore();
}
