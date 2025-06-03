#include "targetdesignationsview.h"

#include <QPainter>

using namespace View;

TargetDesignationsView::TargetDesignationsView(QWidget *parent)
    : QWidget(parent)
{}

void TargetDesignationsView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.fillRect(rect(), QColor("red"));
}
