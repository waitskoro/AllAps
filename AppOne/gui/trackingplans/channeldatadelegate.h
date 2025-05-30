#pragma once

#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QRect>

namespace View {

class ChannelDataDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ChannelDataDelegate(QObject *parent = nullptr);

signals:
    void itemClicked(const int);

private:

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index) override;

    QPersistentModelIndex m_clickedIndex;
};

}
