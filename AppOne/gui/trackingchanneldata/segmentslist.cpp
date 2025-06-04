#include "segmentslist.h"

#include "segmentdelegate.h"

using namespace View;

SegmentsList::SegmentsList(QWidget *parent)
    : QListView(parent)
    , m_headerLabel(new QLabel(this))
    , m_model(new QStandardItemModel(this))
{
    auto *delegate = new SegmentDelegate(this);
    setItemDelegate(delegate);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setSpacing(3);
    setFixedSize(395, 400);
    setModel(m_model.get());
    setStyleSheet("background-color: #EBEBEB");

    m_headerLabel->move(100, 130);

    m_headerLabel->setStyleSheet("background-color: transparent;"
                                 "font-size: 16px;"
                                 "font-weight: 600;");

    m_headerLabel->setText("Для отображения списка \n"
                           "нажмите на один из \n"
                           "элементов списка слева\n"
                           "                      <-");
}

void SegmentsList::clear()
{
    m_model->clear();
    m_headerLabel->show();
}

void SegmentsList::addMessage(const DataChannelSegment &msg, qint32 channelNumber)
{
    auto *item = new QStandardItem();
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    item->setData(msg.physicalChannelNumber, PhysicalChannelNumber);
    item->setData(msg.sectorNumber, SectorNumber);
    item->setData(msg.polarizationDirection, PolarizationDirection);
    item->setData(msg.spacecraftNumber, SpacecraftNumber);
    item->setData(msg.centerFrequency, CenterFrequency);
    item->setData(msg.startTime, StartTime);
    item->setData(msg.endTime, EndTime);
    item->setData(channelNumber, ChannelNumber);
    item->setData(msg.targetCount, TargetCount);

    QVariant variant;
    variant.setValue(msg.targets);
    item->setData(variant, Targets);

    m_headerLabel->hide();

    m_model->appendRow(item);
}
