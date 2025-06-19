#include "logviewer.h"

#include <QPainter>
#include <QCloseEvent>

using namespace View;

LogViewer::LogViewer(QWidget *parent)
    : QWidget(parent)
{
    // setWindowFlags(Qt::Popup);
}

void LogViewer::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QPainter *painter = new QPainter(this);

    painter->fillRect(rect(), "white");

    painter->end();
}
