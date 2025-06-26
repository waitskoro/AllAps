#include "customcombobox.h"

#include <QStylePainter>

using namespace View::Common;

CustomComboBox::CustomComboBox(QWidget *parent)
    : QComboBox(parent)
{
    setFixedWidth(300);
    setStyleSheet("QComboBox { combobox-popup: 0.5px }");

    connect(model(),
            &QAbstractItemModel::rowsInserted,
            this,
            &CustomComboBox::slotModelRowsInserted);

    connect(model(),
            &QAbstractItemModel::rowsRemoved,
            this,
            &CustomComboBox::slotModelRowsRemoved);

    QStandardItemModel *standartModel = qobject_cast<QStandardItemModel*>(model());

    connect(standartModel,
            &QStandardItemModel::itemChanged,
            this,
            &CustomComboBox::slotModelItemChanged);

    for (int i = 1; i <= 12; i++) {
        addItem(QString::number(i));
    }
}

QStringList CustomComboBox::checkedItems() const
{
    return m_checkedItems;
}

void CustomComboBox::setCheckedItems(const QVector<qint8> &items)
{
    QStandardItemModel *standartModel = qobject_cast<QStandardItemModel*>(model());

    disconnect(standartModel,
               &QStandardItemModel::itemChanged,
               this,
               &CustomComboBox::slotModelItemChanged);

    for (int i = 0; i < items.count(); ++i) {
        int index = findText(QString::number(items.at(i)));

        if (index != -1) {
            standartModel->item(index)->setData(Qt::Checked, Qt::CheckStateRole);
        }
    }

    connect(standartModel,
            &QStandardItemModel::itemChanged,
            this,
            &CustomComboBox::slotModelItemChanged);

    collectCheckedItems();
}

void CustomComboBox::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QStylePainter painter(this);
    painter.setPen(palette().color(QPalette::Text));

    QStyleOptionComboBox option;
    initStyleOption(&option);

    painter.drawComplexControl(QStyle::CC_ComboBox, option);

    QRect textRect = rect().adjusted(m_displayRectDelta.left(), m_displayRectDelta.top(),
                                     m_displayRectDelta.right(), m_displayRectDelta.bottom());

    painter.drawText(textRect, Qt::AlignVCenter, m_displayText);
}


void CustomComboBox::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

    updateDisplayText();
}

void CustomComboBox::collectCheckedItems()
{
    QStandardItemModel *standartModel = qobject_cast<QStandardItemModel*>(model());

    m_checkedItems.clear();

    for (int i = 0; i < count(); ++i) {
        QStandardItem *currentItem = standartModel->item(i);

        Qt::CheckState checkState = static_cast<Qt::CheckState>(currentItem->data(Qt::CheckStateRole).toInt());

        if (checkState == Qt::Checked) {
            m_checkedItems.push_back(currentItem->text());
        }
    }

    updateDisplayText();

    repaint();
}

void CustomComboBox::updateDisplayText()
{
    QRect textRect = rect().adjusted(m_displayRectDelta.left(), m_displayRectDelta.top(),
                                     m_displayRectDelta.right(), m_displayRectDelta.bottom());

    QFontMetrics fontMetrics(font());

    m_displayText = m_checkedItems.join(", ");

    if (fontMetrics.size(Qt::TextSingleLine, m_displayText).width() > textRect.width())
    {
        while (m_displayText != "" && fontMetrics.size(Qt::TextSingleLine, m_displayText + "...").width() > textRect.width())
        {
            m_displayText.remove(m_displayText.length() - 1, 1);
        }

        m_displayText += "...";
    }
}

void CustomComboBox::slotModelRowsInserted(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent)

    QStandardItemModel *standartModel = qobject_cast<QStandardItemModel*>(model());

    disconnect(standartModel,
               &QStandardItemModel::itemChanged,
               this,
               &CustomComboBox::slotModelItemChanged);

    for (int i = start; i <= end; ++i) {
        standartModel->item(i)->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        standartModel->item(i)->setData(Qt::Unchecked, Qt::CheckStateRole);
    }

    connect(standartModel,
            &QStandardItemModel::itemChanged,
            this,
            &CustomComboBox::slotModelItemChanged);
}

void CustomComboBox::slotModelRowsRemoved(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent)
    Q_UNUSED(start)
    Q_UNUSED(end)

    collectCheckedItems();
}

void CustomComboBox::slotModelItemChanged(QStandardItem *item)
{
    Q_UNUSED(item)

    collectCheckedItems();
}

