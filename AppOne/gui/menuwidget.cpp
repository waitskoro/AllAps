#include "menuwidget.h"

#include <QRect>
#include <QPainter>

using namespace View;

namespace {

QString baseSetButtonRequest = "border-radius: 5px;"
                               "background-color: #6E87D8;"
                               "font-size: 20px;"
                               "text-align:left;"
                               "vertical-align: middle;";

QString baseSetButtonCommand = "border-radius: 5px;"
                               "background-color: #64D4AD;"
                               "font-size: 20px;"
                               "text-align:left;"
                               "vertical-align: middle;";

}

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget(parent)
    , m_stateWorking(new QPushButton(" Состояния\n работоспособности", this))
    , m_trackingPlans(new QPushButton(" Состояния\n каналов данных", this))
    , m_target(new QPushButton(" Передача\n целеуказаний", this))
    , m_stopingPlans(new QPushButton(" Остановка приема", this))
    , m_main(new QPushButton(" Главное меню ", this))
{
    hide();
    setFixedSize(300, 400);

    m_stateWorking->setFixedSize(230, 50);
    m_stateWorking->setStyleSheet(baseSetButtonRequest);
    m_stateWorking->move(50, 100);

    m_trackingPlans->setFixedSize(230, 50);
    m_trackingPlans->setStyleSheet(baseSetButtonRequest);
    m_trackingPlans->move(50, 155);

    m_target->setFixedSize(230, 50);
    m_target->setStyleSheet(baseSetButtonCommand);
    m_target->move(50, 250);

    m_stopingPlans->setFixedSize(230, 40);
    m_stopingPlans->setStyleSheet(baseSetButtonCommand);
    m_stopingPlans->move(50, 305);

    m_main->move(10, 10);
    m_main->setFixedSize(250, 30);

    connect(m_stateWorking, &QPushButton::clicked, [this](){
        emit sendFeature(StateWorking);
    });

    connect(m_trackingPlans, &QPushButton::clicked, [this](){
        emit sendFeature(TrackingPlans);
    });

    connect(m_target, &QPushButton::clicked, [this](){
        emit sendFeature(TargetDesign);
    });

    connect(m_stopingPlans, &QPushButton::clicked, [this](){
        emit sendFeature(StopingPlans);
    });

    connect(m_main, &QPushButton::clicked, [this](){
        emit sendFeature(Plans);
    });
}

void MenuWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    painter.fillRect(rect(), "#2D2D2D");

    // Title
    QFont font;
    font.setBold(true);
    font.setWeight(QFont::DemiBold);
    font.setPixelSize(30);
    painter.setFont(font);

    // Menu items
    font.setWeight(QFont::Normal);
    font.setPixelSize(24);
    painter.setFont(font);

    auto requests = QRect(30, 60, 300, 50);
    painter.setPen(Qt::white);
    painter.drawText(requests, "Запросы");

    auto commands = QRect(30, 210, 300, 50);
    painter.setPen(Qt::white);
    painter.drawText(commands, "Команды");

    // Title line
    QPen pen = QPen();
    pen.setWidth(2);
    pen.setColor("#708086");
    painter.setPen(pen);

    QLine line = QLine(0, 50, 310, 50);
    painter.drawLine(line);

    // Menu items lines
    QLine lineRequest = QLine(30, 90, 125, 90);
    painter.drawLine(lineRequest);

    QLine lineCommands = QLine(30, 240, 135, 240);
    painter.drawLine(lineCommands);
}
