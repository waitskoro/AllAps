#include "infowidget.h"

#include "infolist.h"

#include <QPainter>

using namespace View;

InfoWidget::InfoWidget(QWidget *parent)
    : QWidget(parent)
    , m_infoList(new InformationList(this))
{
    hide();

    m_infoList->move(10, 10);
}

void InfoWidget::addItem(Report msg)
{
    m_infoList->addInfo(msg);
}

void InfoWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter;

    painter.begin(this);

    painter.fillRect(rect(), QColor(255, 255 ,255, 30));

    painter.end();

    QWidget::paintEvent(e);
}
