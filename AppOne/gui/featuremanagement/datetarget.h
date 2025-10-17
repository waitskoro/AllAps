#pragma once

#include <QWidget>
#include <QDateEdit>

struct Dates {
    QDateTime start;
    QDateTime end;
};

namespace View {

class DateTarget : public QWidget
{
    Q_OBJECT
public:
    explicit DateTarget(QWidget *parent = nullptr);

    Dates dates();

private:
    void init();
    void paintEvent(QPaintEvent *event);

    QDateEdit *m_endDate;
    QTimeEdit *m_endTime;
    QDateEdit *m_startDate;
    QTimeEdit *m_startTime;
};

}
