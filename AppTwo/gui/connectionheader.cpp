#include "connectionheader.h"

#include "connectionheaderblock.h"

#include <QLabel>
#include <QPainter>

using namespace View;

ConnectionHeader::ConnectionHeader(QWidget *parent)
    : QWidget(parent)
    , m_connectBtn(new QPushButton(this))
    , m_host(new QLineEdit(this))
    , m_port(new QLineEdit(this))
    , m_block(new ConnectionHeaderBlock(this))
{
    setFixedSize(700, 150);

    m_connectBtn->move(100, 110);
    m_connectBtn->setFixedSize(240, 25);
    m_connectBtn->setText("Создать сервер");
    m_connectBtn->setStyleSheet("color: #3C388D;"
                                "background-color: white;"
                                "font-weight: 600;");

    m_host->move(150, 40);
    m_port->move(150, 75);

    connect(m_connectBtn,
            &QPushButton::clicked,
            [this]() {
                emit connectTo(m_host->text(), m_port->text());
            });

    m_block->setVisible(false);

    connect(m_block,
            &ConnectionHeaderBlock::disconnect,
            this,
            &ConnectionHeader::onDisconnected);
}

void ConnectionHeader::onConnected()
{
    m_block->setVisible(true);
}

void ConnectionHeader::onDisconnected()
{
    m_block->setVisible(false);
    emit disconnect();
}

void ConnectionHeader::paintEvent(QPaintEvent *e)
{
    QPainter painter;

    painter.begin(this);

    //===========================================

    painter.drawRect(rect());
    painter.fillRect(rect(), "#3C388D");

    QPen pen;
    pen.setWidth(2);
    pen.setColor("#9FC53A");
    painter.setPen(pen);

    //Vertical Lines
    painter.drawLine(450, 30, 450, 149);

    //Text
    pen.setColor("white");
    painter.setPen(pen);

    QFont font;
    font.setBold(true);
    font.setWeight(QFont::Bold);
    painter.setFont(font);

    painter.drawText(110, 58, "IP");
    painter.drawText(110, 93, "Port");

    //===========================================

    painter.end();

    QWidget::paintEvent(e);
}
