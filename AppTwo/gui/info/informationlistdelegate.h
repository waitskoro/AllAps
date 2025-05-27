#pragma once

#include <QObject>
#include <QStyledItemDelegate>

class InformationListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit InformationListDelegate(QObject *parent = nullptr);

private:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
};
