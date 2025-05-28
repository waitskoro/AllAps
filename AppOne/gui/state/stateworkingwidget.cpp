#include "stateworkingwidget.h"

#include <QPainter>

#include <src/contants.h>

using namespace View;

StateWorkingWidget::StateWorkingWidget(QWidget *parent)
    : QWidget(parent)
    , m_label(new QLabel(this))
    , m_exit(new QPushButton(this))
    , m_headerLabel(new QLabel(this))
{
    initUI();
    setFixedSize(Sizes::insideSize());

    connect(m_exit,
            &QPushButton::clicked,
            this,
            &StateWorkingWidget::exit);
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
    pen.setColor("#708086");
    painter.setPen(pen);

    QLine line = QLine(0, 60, rect().right(), 60);
    painter.drawLine(line);
}
