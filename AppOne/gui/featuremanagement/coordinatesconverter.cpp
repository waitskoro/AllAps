#include "coordinatesconverter.h"

#include <QLabel>
#include <QPainter>
#include <QGridLayout>

#include <QIntValidator>

using namespace View;

CoordinatesConverter::CoordinatesConverter(QWidget *parent)
    : QWidget(parent)
    , m_degres(new QDoubleSpinBox(this))
    , m_minutes(new QSpinBox(this))
    , m_seconds(new QDoubleSpinBox(this))
    , m_convertButton(new QPushButton("Конвертировать", this))
{
    setFixedSize(300, 200);

    m_minutes->setRange(0, 59);
    m_seconds->setRange(0, 59.999);
    m_degres->setRange(0.0, 300.0);

    m_degres->setDecimals(2);
    m_seconds->setDecimals(3);

    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setContentsMargins(5, 30, 5, 20);

    gridLayout->addWidget(new QLabel("Конвертер координат"), 0, 0, 1, 4, Qt::AlignHCenter);

    gridLayout->addWidget(new QLabel("Минуты"), 1, 0);
    gridLayout->addWidget(m_minutes, 1, 1);

    gridLayout->addWidget(new QLabel("Секунды"), 1, 2);
    gridLayout->addWidget(m_seconds, 1, 3);

    gridLayout->addWidget(new QLabel("Градус"), 2, 0);
    gridLayout->addWidget(m_degres, 2, 1);

    gridLayout->addWidget(new QLabel(""), 3, 0);

    gridLayout->addWidget(m_convertButton, 4, 0, 1, 4, Qt::AlignHCenter);

    connect(m_convertButton,
            &QPushButton::clicked,
            this,
            &CoordinatesConverter::onClickedButton);
}

void CoordinatesConverter::showWindow(Type type)
{
    m_degres->cleanText();
    m_minutes->cleanText();
    m_seconds->cleanText();

    m_type = type;

    show();
}

void CoordinatesConverter::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.fillRect(rect(), QColor("white"));
}

void CoordinatesConverter::onClickedButton(bool)
{
    if (m_minutes->text() != "" || m_seconds->text() != "" || m_degres->text() != "") {

        auto degres = m_degres->text().toDouble();
        auto minutes = m_minutes->text().toDouble();
        auto seconds = m_seconds->text().toDouble();

        auto val = degres + minutes / 60 + seconds / 3600;

        emit converted(val, m_type);

        hide();
    }
}
