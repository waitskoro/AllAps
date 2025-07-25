#pragma once

#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>

#include "src/messagesinfo.h"
#include "src/statemessage.h"
#include "src/channelmessage.h"
#include "src/featuremanagment/targetdesignationsinfo.h"

namespace View {
class LogViewer;

namespace FeatureManagement {
class TargetDesignationWidget;
}

class Loader;
class PlansWidget;
class StopingPlansWidget;

enum CurrentView {
    Plans,
    StateWorking,
    TrackingPlans,
    TargetDesign,
    StopingPlans
};

class MenuWidget;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);

    void showWindow();
    void addMessage(const ReceivingMessage &);
signals:
    void disconnect();

    void sendStopingPlans();

    void sendRequestState();
    void sendRequestTrackingPlans();
    void sendTarget(Application::TargetDesignations target);

    void recieveState(StateMessage &);
    void executedTheCommandRecevied(ExecutedTheCommand result);
    void dataChannelMessagesRecieved(DataChannelMessage &messages);

private:
    QRect m_mainRect;
    QHBoxLayout *m_mainLayout;
    MenuWidget *m_menuWidget;
    QPushButton *m_menuButton;

    PlansWidget *m_plansWidget;

    void drawMenuButton();

    void onGoBack();
    void onButtonMenuClicked(CurrentView);

    Loader *m_loader;
    StopingPlansWidget *m_stopingPlans;

    QWidget* m_currentWidget = nullptr;
    CurrentView m_currentView;

};

}
