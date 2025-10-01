#include "datetarget.h"

#include <QLabel>
#include <QPainter>
#include <QBoxLayout>

using namespace View;

DateTarget::DateTarget(QWidget *parent)
    : QWidget(parent)
    , m_endDate(new QDateEdit(this))
    , m_endTime(new QTimeEdit(this))
    , m_startDate(new QDateEdit(this))
    , m_startTime(new QTimeEdit(this))
{
    init();
}

void DateTarget::init()
{
    QGridLayout *gridLayout = new QGridLayout(this);

    setStyleSheet("QLabel { background: transparent; }");

    gridLayout->addWidget(new QLabel("Начало плана"), 0, 0);
    gridLayout->addWidget(m_startDate, 0, 1);
    gridLayout->addWidget(m_startTime, 0, 2);

    gridLayout->addWidget(new QLabel("Конец плана"), 0, 3);
    gridLayout->addWidget(m_endDate, 0, 4);
    gridLayout->addWidget(m_endTime, 0, 5);

    m_endDate->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_endTime->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_startDate->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_startTime->setButtonSymbols(QAbstractSpinBox::NoButtons);

    m_startDate->setDisplayFormat("MM/dd/yy");
    m_startTime->setDisplayFormat("hh:mm:ss");
    m_endDate->setDisplayFormat("MM/dd/yy");
    m_endTime->setDisplayFormat("hh:mm:ss");

    m_startDate->setDate(QDate::currentDate());
    m_startTime->setTime(QTime::currentTime());
    m_endDate->setDate(QDate::currentDate());
    m_endTime->setTime(QTime::currentTime().addSecs(3600));
}

void DateTarget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.fillRect(rect(), "#F2F0F0");
}

Dates DateTarget::dates()
{
    Dates dates;
    dates.start = QDateTime(m_startDate->date(), m_startTime->time());
    dates.end = QDateTime(m_endDate->date(), m_endTime->time());

    return dates;
}
