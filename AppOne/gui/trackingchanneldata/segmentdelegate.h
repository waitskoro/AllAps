#pragma once

#include <QModelIndex>
#include <QStyledItemDelegate>

namespace View {

class SegmentDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SegmentDelegate(QObject *parent = nullptr);

signals:
    void itemClicked(int);

private:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
};

}
