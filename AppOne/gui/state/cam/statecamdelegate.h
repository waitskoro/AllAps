#pragma once

#include <QModelIndex>
#include <QStyledItemDelegate>

namespace View {

class StateCamDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit StateCamDelegate(QObject *parent = nullptr);

private:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
};

}
