#include "planswidget.h"

#include <src/contants.h>
#include "gui/plansmanagment/planslist.h"

#include <QPainter>

using namespace View;

PlansWidget::PlansWidget(QWidget *parent)
    : QWidget(parent)
    , m_receivingChecklist(new PlansList(this))
    , m_headerLabel(new QLabel("Список приемов пуст", this))
{
    setFixedSize(Sizes::insideSize());
    m_receivingChecklist->move(50, 50);

    m_headerLabel->setStyleSheet("background-color: transparent;"
                                 "font-size: 26px;"
                                 "text-align: center;");

    m_headerLabel->move(rect().width() / 2 - (m_headerLabel->width() + 30),
                        rect().height() / 2 - m_headerLabel->height() / 2);
}

void PlansWidget::addMessage(const ReceivingMessage& msg)
{
    if (m_receivingChecklist) {
        m_receivingChecklist->addMessage(msg);
        m_headerLabel->hide();
    }
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
