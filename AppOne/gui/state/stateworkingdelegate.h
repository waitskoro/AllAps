#pragma once

#include <QModelIndex>
#include <QStyledItemDelegate>

namespace View {

class StateWorkingDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit StateWorkingDelegate(QObject *parent = nullptr);

signals:
    void itemClicked(int);

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
