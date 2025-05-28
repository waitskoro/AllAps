#include "stateworkingwidget.h"

#include <src/contants.h>
#include "stateworkinglist.h"

#include <QPainter>

using namespace View;

StateWorkingWidget::StateWorkingWidget(QWidget *parent)
    : QWidget(parent)
    , m_label(new QLabel(this))
    , m_exit(new QPushButton(this))
    , m_headerLabel(new QLabel(this))
    , m_stateWorkingList(new StateWorkingList(this))
{
    initUI();
    setFixedSize(Sizes::insideSize());

    connect(m_exit,
            &QPushButton::clicked,
            this,
            &StateWorkingWidget::exit);

    m_label->show();
    m_stateWorkingList->hide();
    m_stateWorkingList->move(50, 230);
}

void StateWorkingWidget::addState(StateMessage &state)
{
    m_state = state;

    m_label->hide();
    m_stateWorkingList->show();
    m_stateWorkingList->addState(state);
}

void StateWorkingWidget::initUI()
{
    m_exit->move(25, 15);
    m_exit->setText(" Назад ");
    m_exit->setStyleSheet("background-color: #CEA898;"
                          "font-size: 20px;");

    //--------------------------------------------------------------

    m_label->setStyleSheet("background-color: transparent;"
                           "font-size: 26px;");
    m_label->setText("Ожидание получения данных\n"
                     "  о текущем состоянии АС");

    m_label->move(rect().left() + 180, 150);

    //--------------------------------------------------------------

    m_headerLabel->setStyleSheet("background-color: transparent;"
                                 "font-size: 30px;"
                                 "font-weight: 600;");
    m_headerLabel->setText("Информация о текущем состоянии АС");

    m_headerLabel->move(rect().left() + 120, 15);
}

void StateWorkingWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    painter.fillRect(rect(), QColor("#C3D7E4"));

    QPen pen = QPen();
    pen.setWidth(2);
    pen.setColor("black");
    painter.setPen(pen);

    QLine line = QLine(0, 60, rect().right(), 60);
    painter.drawLine(line);

    pen.setColor("black");
    painter.setPen(pen);

    QFont font;
    font.setPixelSize(18);
    painter.setFont(font);

    painter.drawText(50, 95, "Состояние работоспособности АС");

    font.setPixelSize(16);
    painter.setFont(font);

    painter.drawText(70, 130, QString("Состояние ЭВМ"));
    painter.drawText(70, 150, QString("Состояние АС в целом"));
    painter.drawText(70, 170, QString("Состояние коммутатора 1"));
    painter.drawText(70, 190, QString("Состояние коммутатора 2"));
    painter.drawText(70, 210, QString("Состояние стандарта частоты"));

    painter.drawText(300, 130, "🟢");
    painter.drawText(300, 150, "🟢");
    painter.drawText(300, 170, "🟢");
    painter.drawText(300, 190, "🟢");
    painter.drawText(300, 210, "🟢");

    painter.drawText(320, 130, "В норме");
    painter.drawText(320, 150, "В норме");
    painter.drawText(320, 170, "В норме");
    painter.drawText(320, 190, "В норме");
    painter.drawText(320, 210, "В норме");

    pen.setColor("#708086");
    painter.setPen(pen);

    painter.drawLine(50, 105, 340, 105);
}

void StateWorkingWidget::drawStates(StateRole state, QPainter, QPoint, qint32 stateInfo)
{
    switch (state) {
        case Computer:
            break;

        case AC:
            break;

        case Switch_1:
            break;

        case Switch_2:
            break;

        case Frequency:
            break;

        default:
            break;
    }
}
