#pragma once

#include <QComboBox>
#include <QStandardItem>

namespace View::Common {

class CustomComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit CustomComboBox(QWidget *parent = nullptr);

signals:
    void checkedChannelsChanged(QVector<qint8> &);

private:
    QStandardItemModel *m_model;
    QVector<qint8> *m_checkedChannels;

    qint8 getChannelNumber(QString str);
    void onModelItemChanged(QStandardItem *item);

};

}
