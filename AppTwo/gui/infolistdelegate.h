#pragma once

#include <QObject>
#include <QStyledItemDelegate>

class InfoListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit InfoListDelegate(QObject *parent = nullptr);

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
