#include "customcombobox.h"

#include <QStylePainter>

using namespace View::Common;

CustomComboBox::CustomComboBox(QWidget *parent)
    : QComboBox(parent)
    , m_checkedChannels(new QList<qint8>())
{
    setFixedWidth(300);

    m_model = new QStandardItemModel(13, 1);

    for (int i = 0; i < 13; ++i)
    {
        QStandardItem* item;

        if (i == 0)
            item = new QStandardItem(QString("Выбраны все каналы"));
        else
            item = new QStandardItem(QString("Канал: %0").arg(i));

        item->setData(Qt::Unchecked, Qt::CheckStateRole);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);

        m_model->setItem(i, 0, item);
    }

    setModel(m_model);

    connect(m_model,
            &QStandardItemModel::itemChanged,
            this,
            &CustomComboBox::onModelItemChanged);
}

qint8 CustomComboBox::getChannelNumber(QString str)
{
    int countSymbols = str.length();
    int channel = str.mid(countSymbols - 2, countSymbols).toInt();

    return channel;
}

void CustomComboBox::onModelItemChanged(QStandardItem *item)
{
    m_checkedChannels->clear();

    const bool isSelectAll = (item == m_model->item(0, 0));
    const Qt::CheckState targetState = item->checkState();

    if (isSelectAll)
        m_model->blockSignals(true);

    for (int i = 1; i < 13; ++i) {
        if (isSelectAll)
            m_model->item(i, 0)->setCheckState(targetState);

        if (m_model->item(i, 0)->checkState() == Qt::Checked)
            m_checkedChannels->append(i);
    }

    if (isSelectAll) {
        m_model->item(0, 0)->setCheckState(targetState);
        m_model->blockSignals(false);
    }

    emit checkedChannelsChanged(*m_checkedChannels);
}
