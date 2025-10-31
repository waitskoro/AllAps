#pragma once

#include <QLabel>
#include <QObject>
#include <QStyledItemDelegate>

class InfoListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit InfoListDelegate(QWidget *parent = nullptr);

signals:
    void itemClicked(std::vector<std::array<short, 2>>);

private:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index) override;

    QDateTime fromDoubleToDate(double dateValue) const;
};
