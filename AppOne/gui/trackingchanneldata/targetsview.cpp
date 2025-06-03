#include "targetsview.h"

#include <QPainter>

#include "src/featuremanagment/targetdesignationmodel.h"

using namespace View;

TargetsView::TargetsView(Target target, QWidget *parent)
    : QWidget(parent)
    , m_target(target)
    , m_tableView(new QTableView(this))
    , m_model(new TargetDesignationModel(this))
{
    setFixedSize(450, 200);

    m_tableView->setModel(m_model);

    m_tableView->setFixedSize(300, 170);
    m_tableView->setColumnWidth(0, 130);
    m_tableView->setColumnWidth(1, 150);
    m_tableView->move(140, 10);

    setWindowTitle(QString("Канал данных: %1").arg(target.channelData));
}

void TargetsView::setTargets(std::vector<std::array<short, 2>> &data)
{
    for (size_t i = 0; i < data.size(); i++) {
        m_model->append(data[i]);
    }
}

void TargetsView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter *painter = new QPainter(this);

    painter->fillRect(rect(), QColor(159, 197, 58, 50));

    QFont font;
    font.setPixelSize(16);
    painter->setFont(font);

    QString spacecraft = QString::number(m_target.ka);
    QString sector = QString::number(m_target.sector);
    QString physicalChannel = QString::number(m_target.physicalChannel);

    QRect *rectSpacecraft = new QRect(20, 10, 100, 50);
    QRect *rectSector = new QRect(20, 70, 100, 50);
    QRect *rectPhysicalChannel = new QRect(20, 130, 100, 50);

    painter->fillRect(*rectSpacecraft, "#6E87D8");
    painter->fillRect(*rectSector, "#6E87D8");
    painter->fillRect(*rectPhysicalChannel, "#6E87D8");

    QString titleSpacecraft = "КА";
    QString titleSector = "Сектор";
    QString titlePhysicalChannel = "Физ. канал";

    drawCommonInfo(painter,
                   rectSpacecraft,
                   titleSpacecraft,
                   spacecraft);

    drawCommonInfo(painter,
                   rectSector,
                   titleSector,
                   sector);

    drawCommonInfo(painter,
                   rectPhysicalChannel,
                   titlePhysicalChannel,
                   physicalChannel);

    painter->end();
}

void TargetsView::drawCommonInfo(QPainter *painter,
                                 QRect *rect,
                                 QString &title,
                                 QString &info) const
{
    QPen pen;
    pen.setColor("white");
    painter->setPen(pen);

    painter->drawText(QRect(20, rect->top() + 5,
                            rect->width(), rect->height()),
                      Qt::AlignHCenter,
                      title);

    painter->drawText(QRect(20, rect->top() + 25,
                            rect->width(), rect->height()),
                      Qt::AlignHCenter,
                      info);
}
