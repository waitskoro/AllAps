#pragma once

#include <QTableView>
#include <QModelIndex>
#include <QPushButton>
#include <QStyledItemDelegate>

namespace View {

class SegmentDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SegmentDelegate(QWidget *parent = nullptr);

private:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

    bool editorEvent(QEvent *event,
                     QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index) override;

    QPushButton *m_targetView;
    std::vector<std::array<short, 2>> m_values;
};

}
