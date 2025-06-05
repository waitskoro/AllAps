#include "serverconnectingwidget.h"

#include <QLabel>
#include <QPainter>
#include <QPainterPath>

using namespace View;

ServerConnectingWidget::ServerConnectingWidget(QWidget *parent)
    : QWidget(parent)
    , m_host(new QLineEdit(this))
    , m_port(new QLineEdit(this))
    , m_connectButton(new QPushButton("Создать сервер", this))
{
    setFixedSize(625, 50);

    m_host->setText("localhost");
    m_port->setText("4444");

    m_host->setPlaceholderText("IP");
    m_port->setPlaceholderText("Port");

    m_host->setFixedSize(145, 25);
    m_port->setFixedSize(70, 25);

    m_host->move(15, 12);
    m_port->move(m_host->rect().right() + 40, 12);

    m_connectButton->move(280, 12);
    m_connectButton->setFixedSize(150, 25);
    m_connectButton->setStyleSheet("background-color: #3354A1;"
                                   "color: white;"
                                   "border-radius: 3px;"
                                   "font-weight: 600;");

    connect(m_connectButton, &QPushButton::clicked, [this](){
        int port = m_port->text().toInt();
        emit createServer(m_host->text(), port);
    });
}

void ServerConnectingWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter;
    painter.begin(this);

    //===========================================

    painter.setRenderHint(QPainter::Antialiasing);

    //---------fill rect with radius------------
    //--Main--
    QPainterPath path;
    path.addRoundedRect(rect(), 10, 10);
    painter.fillPath(path, QBrush("white"));
    painter.drawPath(path);

    QPen pen;
    pen.setColor("#3354A1");
    painter.setPen(pen);

    painter.drawLine(rect().right() - 160, 8,
                     rect().right() - 160, rect().bottom() - 8);

    //===========================================


    painter.end();

    QWidget::paintEvent(e);
}
