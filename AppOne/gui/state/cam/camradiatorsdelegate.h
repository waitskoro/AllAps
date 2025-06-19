#pragma once

#include <QObject>
#include <QStyledItemDelegate>

namespace View {

class CamRadiatorsDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CamRadiatorsDelegate(QObject *parent = nullptr);

private:
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
};

}
