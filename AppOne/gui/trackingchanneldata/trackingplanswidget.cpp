#include "trackingplanswidget.h"

#include <src/contants.h>
#include "segmentslist.h"
#include "channeldatalist.h"

#include <QPainter>

using namespace View;

TrackingPlansWidget::TrackingPlansWidget(QWidget *parent)
    : QWidget(parent)
    , m_label(new QLabel(this))
    , m_exit(new QPushButton(this))
    , m_headerLabel(new QLabel(this))
    , m_segmentsList(new SegmentsList(this))
    , m_channelDataTable(new ChannelDataList(this))
{
    setFixedSize(Sizes::insideSize());

    m_segmentsList->setVisible(false);
    m_channelDataTable->setVisible(false);

    initUI();

    connect(m_exit,
            &QPushButton::clicked,
            this,
            &TrackingPlansWidget::goBack);

    connect(m_channelDataTable,
            &ChannelDataList::itemClicked,
            [this](const int channelNumber){
                m_segmentsList->clear();

                for (int i = 0; i < m_message.activeChannelsCount; i++) {

                    auto &ch = m_message.channels[i];

                    if (ch.channelNumber == channelNumber) {

                        for (int j = 0; j < ch.segmentCount; j++) {
                            m_segmentsList->addMessage(ch.segments[j], channelNumber);
                        }
                    }
                }

            });

    m_currentState = Waiting;
}

void TrackingPlansWidget::addMessage(DataChannelMessage &message)
{
    if (message.activeChannelsCount == 0)
        return;

    m_message = message;

    m_segmentsList->setVisible(true);
    m_channelDataTable->setVisible(true);

    m_channelDataTable->clear();

    for (int i = 0; i < message.activeChannelsCount; i++) {
        m_channelDataTable->addMessage(message.channels[i]);
    }

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
    m_label->setText("Для отображения информации\n"
                     "        создайте целеуказание");

    m_label->move(centerX(rect().size(), QSize(350, 50)), 150);

    //--------------------------------------------------------------

    m_headerLabel->setStyleSheet("background-color: transparent;"
                                 "font-size: 30px;"
                                 "font-weight: 600;");
    m_headerLabel->setText("Cостояние каналов данных");

    m_headerLabel->move(centerX(rect().size(), QSize(370, 50)), 15);

    //Table
    m_channelDataTable->move(20, 70);
    m_segmentsList->move(285, 70);

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
