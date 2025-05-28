#include "trackingplanswidget.h"

#include <QPainter>

#include <src/contants.h>
#include <gui/trackingplans/channeldatalist.h>

using namespace View;

TrackingPlansWidget::TrackingPlansWidget(QWidget *parent)
    : QWidget(parent)
    , m_label(new QLabel(this))
    , m_exit(new QPushButton(this))
    , m_headerLabel(new QLabel(this))
    , m_channelDataTable(new ChannelDataList(this))
{
    setFixedSize(Sizes::insideSize());
    m_channelDataTable->setVisible(false);

    initUI();

    connect(m_exit,
            &QPushButton::clicked,
            this,
            &TrackingPlansWidget::goBack);

    m_currentState = Waiting;
}

void TrackingPlansWidget::addMessage(DataChannelMessage &message)
{
    m_channelDataTable->setVisible(true);
    m_channelDataTable->addMessage(message);

    m_label->setVisible(false);
}

void TrackingPlansWidget::initUI()
{
    // Buttons
    m_exit->move(25, 15);
    m_exit->setText(" Назад ");
    m_exit->setStyleSheet("background-color: #CEA898;"
                          "font-size: 20px;");

    //Labels
    m_label->setStyleSheet("background-color: transparent;"
                           "font-size: 26px;");
    m_label->setText("Ожидание получения данных\n"
                     "об активных каналах данных");

    m_label->move(centerX(rect().size(), QSize(350, 50)), 150);

    //--------------------------------------------------------------

    m_headerLabel->setStyleSheet("background-color: transparent;"
                                 "font-size: 30px;"
                                 "font-weight: 600;");
    m_headerLabel->setText("Cостояние каналов данных");

    m_headerLabel->move(centerX(rect().size(), QSize(370, 50)), 15);

    //Table
    m_channelDataTable->move(50, 70);

}

void TrackingPlansWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.fillRect(rect(), QColor("#C3D7E4"));

    QPen pen = QPen();
    pen.setWidth(2);
    pen.setColor("#708086");
    painter.setPen(pen);

    QLine line = QLine(0, 60, rect().right(), 60);
    painter.drawLine(line);
}
