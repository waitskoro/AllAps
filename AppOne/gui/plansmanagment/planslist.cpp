#include "planslist.h"
#include "planslistdelegate.h"
#include <QTimer>
#include <QDebug>

using namespace View;

PlansList::PlansList(QWidget *parent)
    : QListView(parent)
    , isNew(false)
    , m_clearTimer(new QTimer(this))
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

    m_clearTimer->setSingleShot(true);
    connect(m_clearTimer.get(), &QTimer::timeout, this, [this]() {
        m_model->clear();
        emit messagesIsEmpty();
    });

    setStyleSheet("background-color: #C3D7E4");
}

PlansList::~PlansList()
{
    m_isActive = false;
    disconnect(m_refreshTimer.data(), nullptr, this, nullptr);
}

void PlansList::addMessage(const ReceivingMessage& msg)
{
    m_clearTimer->start(3500);

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

        item->setData(msg.coordinates[0] / 10, CurrentAzimut);
        item->setData(msg.coordinates[1] / 10, CurrentBeanAzimut);

        item->setData(msg.state, State);
        item->setData(msg.levelOfSignal, LevelOfSignal);

        qint32 azimutEndSector = msg.azimutEndSector / 10;
        qint32 azimutStartSector = msg.azimutStartSector / 10;

        item->setData(azimutEndSector, AzimutEndSector);
        item->setData(azimutStartSector, AzimutStartSector);
        item->setData(msg.receivingSectorNumber, ReceivingSectorNumber);
        item->setData(msg.directionOfPolarizaion, DirectionOfPolarizaion);

        m_model->appendRow(item);
    });
}

void PlansList::clear()
{
    if (!m_isActive || m_model.isNull()) {
        return;
    }
    m_model->clear();
}
