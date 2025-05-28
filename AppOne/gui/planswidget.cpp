#include "planswidget.h"

#include <src/contants.h>
#include "gui/plansmanagment/planslist.h"

#include <QPainter>

using namespace View;

PlansWidget::PlansWidget(QWidget *parent)
    : QWidget(parent)
    , m_receivingChecklist(new PlansList(this))
{
    setFixedSize(Sizes::insideSize());
    m_receivingChecklist->move(50, 50);
}

void PlansWidget::addMessage(const ReceivingMessage& msg)
{
    if (m_receivingChecklist)
        m_receivingChecklist->addMessage(msg);
}

void PlansWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.fillRect(rect(), QColor("#C3D7E4"));

    QFont font;
    font.setBold(true);
    font.setWeight(QFont::DemiBold);
    font.setPixelSize(22);
    painter.setFont(font);

    painter.drawText(50, 30, "Состояния приема");

    QPen pen;
    pen.setColor("white");
    pen.setWidth(2);
    painter.setPen(pen);

    painter.drawLine(0, 40, 750, 40);
}
