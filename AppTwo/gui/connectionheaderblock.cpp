#include "connectionheaderblock.h"

#include <QLabel>
#include <QPainter>
#include <QPushButton>

using namespace View;

ConnectionHeaderBlock::ConnectionHeaderBlock(QWidget *parent)
    : QWidget(parent)
{
    QWidget *widget = new QWidget(this);
    widget->setFixedSize(449, 147);
    widget->setStyleSheet("background-color: #3C388D");

    QLabel *l = new QLabel(this);
    l->move(135, 20);
    l->setFixedSize(200, 100);
    l->setText("Сервер запущен");
    l->setStyleSheet("background-color: transparent;"
                     "font-size: 22px;"
                     "color: white;"
                     "font-weight: 600;");

    QPushButton *m_exit = new QPushButton(this);
    m_exit->setFixedSize(150, 30);
    m_exit->move(150, 100);
    m_exit->setText("Отключиться");
    m_exit->setStyleSheet("color: #3C388D;"
                          "background-color: white;"
                          "font-weight: 600;");

    connect(m_exit,
            &QPushButton::clicked,
            this,
            &ConnectionHeaderBlock::disconnect);
}
