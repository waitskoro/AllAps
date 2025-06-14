#include "planslist.h"
#include "planslistdelegate.h"
#include <QTimer>
#include <QDebug>

using namespace View;

PlansList::PlansList(QWidget *parent)
    : QListView(parent)
    , isNew(false)
    , m_refreshTimer(new QTimer(this))
    , m_model(new QStandardItemModel(this))
{
    auto *delegate = new PlansListDelegate(this);
    setItemDelegate(delegate);

    setSpacing(5);
    setModel(m_model.get());
    setFixedWidth(600);
    setFixedHeight(420);

    connect(m_refreshTimer.get(), &QTimer::timeout, this, [this]() {
        isNew = true;
    });
    m_refreshTimer->start(1500);

    setStyleSheet("background-color: #C3D7E4");
}

PlansList::~PlansList()
{
    m_isActive = false;
    disconnect(m_refreshTimer.data(), nullptr, this, nullptr);
}

void PlansList::addMessage(const ReceivingMessage& msg)
{
    QMetaObject::invokeMethod(this, [this, msg]() {
    if (!m_isActive || m_model.isNull()) return;

    if (isNew) {
        m_model->clear();
        isNew = false;
    }
        if (!m_isActive) return;

        auto *item = new QStandardItem();
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        item->setData(msg.channelNumber, ChannelNumber);
        item->setData(msg.centerFrequency, CenterFrequency);
        item->setData(msg.spacecraftNumber, SpacecraftNumber);

        item->setData(msg.coordinates[0], CurrentAzimut);
        item->setData(msg.coordinates[1], CurrentBeanAzimut);

        item->setData(msg.state, State);
        item->setData(msg.levelOfSignal, LevelOfSignal);
        item->setData(msg.azimutEndSector, AzimutEndSector);
        item->setData(msg.azimutStartSector, AzimutStartSector);
        item->setData(msg.receivingSectorNumber, ReceivingSectorNumber);
        item->setData(msg.directionOfPolarizaion, DirectionOfPolarizaion);

        m_model->appendRow(item);
    });
}

void PlansList::emptyMessages()
{
    if (!m_isActive || m_model.isNull()) {
        return;
    }
    m_model->clear();
}
