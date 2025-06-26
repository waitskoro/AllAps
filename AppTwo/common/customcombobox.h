#pragma once

#include <QComboBox>
#include <QStandardItem>

namespace View::Common {

class CustomComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit CustomComboBox(QWidget *parent = nullptr);

    QStringList checkedItems() const;
    void setCheckedItems(const QVector<qint8> &items);

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

private:
    QString m_displayText;
    QStringList m_checkedItems;
    const QRect m_displayRectDelta;

    void updateDisplayText();
    void collectCheckedItems();

private slots:
    void slotModelItemChanged(QStandardItem *item);
    void slotModelRowsRemoved(const QModelIndex &parent, int start, int end);
    void slotModelRowsInserted(const QModelIndex &parent, int start, int end);
};

}
