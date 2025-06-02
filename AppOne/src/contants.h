#pragma once

#include <QSize>
#include <QPoint>
#include <QColor>
#include <QDateTime>
#include <QPushButton>

namespace Sizes {

inline QSize authSize() {
    return QSize(650, 400);
}

inline QSize mainSize() {
    return QSize(800, 600);
}

inline QSize insideSize() {
    return QSize(700, 500);
}

}

namespace Styles {

inline QString mainColor() {
    return "background-color: #526879";
}

}

namespace {

inline QPoint centeringCoordinates(QSize big, QSize small) {
    const int x = (big.width() - small.width()) / 2;
    const int y = (big.height() - small.height()) / 2;

    return QPoint(x, y);
}

inline int centerX(QSize big, QSize small)  {
    return (big.width() - small.width()) / 2;
}

inline int centerY(QSize big, QSize small) {
    return (big.height() - small.height()) / 2;
}

inline QPushButton* createButton(QWidget *parent, const QString& text, int width, int height)
{
    auto button = new QPushButton(text, parent);
    button->setFixedSize(width, height);
    button->setStyleSheet(
        "background-color: #2C4B65;"
        "color: white;"
        "font-size: 14pt;"
        "font-weight: 500;"
        );
    return button;
}

inline int fromStrToInt(QString str)
{
    bool ok;
    int num = str.toInt(&ok);
    if (ok)
        return num;

    return -1;
}

inline double toDouble(const QString &str)
{
    bool ok;
    return str.toDouble(&ok);
}

inline double fromDateToDouble(QDateTime date)
{
    return double(date.toSecsSinceEpoch()) / double (86400) + 25569;
}

inline double currentOADate()
{
    QDateTime now = QDateTime::currentDateTime();
    return double(now.toSecsSinceEpoch()) / double (86400) + 25569;
}

inline QDateTime fromDoubleToDate(double dateValue)
{
    const double unixTime = (dateValue - 25569) * 86400;
    return QDateTime::fromSecsSinceEpoch(static_cast<qint64>(unixTime));
}

}
