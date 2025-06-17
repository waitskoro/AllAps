#include "logviewer.h"

#include <QPainter>

using namespace View;

LogViewer::LogViewer(QWidget *parent)
    : QWidget(parent)
{
    show();
}

void LogViewer::paintEvent(QPaintEvent *e)
{
    QPainter *painter = new QPainter(this);
    painter->begin(this);

    painter->fillRect(rect(), "white");

    painter->end();
}
