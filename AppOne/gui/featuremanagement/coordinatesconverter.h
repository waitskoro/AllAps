#pragma once

#include <QWidget>
#include <QSpinBox>
#include <QPushButton>

namespace View {

class CoordinatesConverter : public QWidget
{
    Q_OBJECT
public:

    enum Type {
        Lat,
        Lon
    };

    explicit CoordinatesConverter(QWidget *parent = nullptr);
    void showWindow(Type m_type);
signals:
    void converted(double coord, Type type);

private:
    void paintEvent(QPaintEvent *event);

    QDoubleSpinBox *m_degres;
    QSpinBox *m_minutes;
    QDoubleSpinBox *m_seconds;

    QPushButton *m_convertButton;

    void onClickedButton(bool);

    Type m_type;
};

}
