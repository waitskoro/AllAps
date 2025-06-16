#pragma once

#include <QObject>
#include <QStyledItemDelegate>

namespace View {

class PlansListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit PlansListDelegate(QObject *parent = nullptr);

private:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
};

}
