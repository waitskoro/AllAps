#include "mainwidget.h"

#include "menuwidget.h"
#include "src/contants.h"

#include "loader.h"
#include "planswidget.h"
#include "state/stateworkingwidget.h"
#include "stopingplans/stopingplanswidget.h"
#include "trackingchanneldata/trackingplanswidget.h"
#include "featuremanagement/targetdesignationwidget.h"

#include <QPainter>
#include <QMessageBox>

using namespace View;
using namespace View::FeatureManagement;

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , m_mainRect(QRect(0, 0, 700, 500))
    , m_mainLayout(new QHBoxLayout(this))
    , m_menuWidget(new MenuWidget(this))
    , m_menuButton(new QPushButton(this))
    , m_plansWidget(new PlansWidget(this))
    , m_loader(new Loader(QString("Ожидание ответа"),
                        QRect(0, 0, 800, 600),
                        this))
    , m_stopingPlans(new StopingPlansWidget(this))
{
    m_currentView = Plans;

    m_loader->close();

    m_mainLayout->addWidget(m_plansWidget);

    setFixedSize(Sizes::mainSize());
    setStyleSheet("background-color: #526879");

    m_mainRect.moveTo(centeringCoordinates(Sizes::mainSize(),
                                           m_mainRect.size()));

    m_menuWidget->move(10, 50);
    m_menuWidget->setAttribute(Qt::WA_AlwaysStackOnTop);

    connect(m_menuButton, &QPushButton::clicked, [this](){
        if (m_menuWidget->isVisible()) {
            m_menuWidget->close();
        } else {
            QPoint pos = m_menuButton->mapToGlobal(QPoint(0, m_menuButton->height()));
            m_menuWidget->move(pos);
            m_menuWidget->show();
        }
    });
    connect(this, &MainWidget::executedTheCommandRecevied, [this](ExecutedTheCommand result){
                if (m_loader->isVisible())
                    m_loader->stop();

                if (m_stopingPlans->isVisible())
                    m_stopingPlans->messageRecivied(result);
            });

    connect(m_menuWidget, &MenuWidget::disconnect, [this](){
                m_menuWidget->close();
                onGoBack();
                emit disconnect();
            });

    setLayout(m_mainLayout);

    drawMenuButton();

    connect(m_menuWidget,
            &MenuWidget::sendFeature,
            this,
            &MainWidget::onButtonMenuClicked);

    connect(m_stopingPlans, &StopingPlansWidget::send, [this]() {
                emit sendStopingPlans();
            });

    connect(m_stopingPlans, &StopingPlansWidget::canceled, [this]() {
                m_currentView = Plans;
            });

    connect(m_stopingPlans, &StopingPlansWidget::stoped, [this]() {
                m_currentView = Plans;
            });

    m_menuWidget->setWindowFlags(Qt::Popup);
    m_menuWidget->setAttribute(Qt::WA_TranslucentBackground);
}

void MainWidget::showWindow()
{
    show();
}

void MainWidget::onRemoteHostClosed()
{
    m_menuWidget->close();
    onGoBack();
    emit disconnect();
}

void MainWidget::addMessage(const ReceivingMessage &msg)
{
    m_plansWidget->addMessage(msg);
}

void MainWidget::drawMenuButton()
{
    m_menuButton->setFixedSize(40, 40);
    m_menuButton->move(10,10);

    QPixmap pixmap(24, 24);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::white, 2));
    painter.drawLine(2, 6, 22, 6);
    painter.drawLine(2, 12, 22, 12);
    painter.drawLine(2, 18, 22, 18);

    m_menuButton->setIcon(QIcon(pixmap));
    m_menuButton->setIconSize(QSize(24, 24));
}

void MainWidget::onButtonMenuClicked(CurrentView nextView)
{
    if (nextView == m_currentView) {
        m_menuWidget->close();
        return;
    } else {
        m_currentView = nextView;
    }

    QLayoutItem* item;
    while ((item = m_mainLayout->takeAt(0)) != nullptr) {
        delete item;
    }

    m_menuWidget->close();

    switch (nextView) {
        case Plans: {
            m_plansWidget = new PlansWidget(this);
            m_mainLayout->addWidget(m_plansWidget);
            break;
        }
        case StateWorking: {
            auto state = new StateWorkingWidget(this);
            m_mainLayout->addWidget(state);

            connect(state,
                    &StateWorkingWidget::exit,
                    this,
                    &MainWidget::onGoBack);

            connect(this,
                    &MainWidget::recieveState,
                    state,
                    &StateWorkingWidget::addState);

            emit sendRequestState();
            break;
        }
        case TrackingPlans: {
            auto trackingPlansWidget = new TrackingPlansWidget(this);
            m_mainLayout->addWidget(trackingPlansWidget);

            connect(trackingPlansWidget,
                    &TrackingPlansWidget::goBack,
                    this,
                    &MainWidget::onGoBack);

            connect(this,
                    &MainWidget::dataChannelMessagesRecieved,
                    trackingPlansWidget,
                    &TrackingPlansWidget::addMessage);

            emit sendRequestTrackingPlans();

            break;
        }
        case TargetDesign: {
            auto *target = new TargetDesignationWidget(this);
            m_mainLayout->addWidget(target);

            connect(target,
                    &TargetDesignationWidget::goBack,
                    this,
                    &MainWidget::onGoBack);

            connect(target,
                    &TargetDesignationWidget::createTarget,
                    this,
                    [this](Application::TargetDesignations target) {
                        onGoBack();
                        emit sendTarget(target);

                        m_loader->raise();
                        m_loader->show();
                        m_loader->start();
                    });

            connect(target,
                    &TargetDesignationWidget::createTargetTest,
                    this,
                    [this](Application::TargetDesignations target) {
                        emit sendTarget(target);
                    });
            break;
        }
        case StopingPlans: {
            m_stopingPlans->raise();
            m_stopingPlans->showWindow();

            break;
        }
    }
}

void MainWidget::onGoBack()
{
    QLayoutItem* item;
    while ((item = m_mainLayout->takeAt(0)) != nullptr) {
        delete item;
    }

    m_plansWidget = new PlansWidget(this);
    m_mainLayout->addWidget(m_plansWidget);

    m_currentView = Plans;
}
