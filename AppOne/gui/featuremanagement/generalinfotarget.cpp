#include "generalinfotarget.h"

#include <QLabel>
#include <QPainter>
#include <QBoxLayout>

using namespace View;

GeneralInfoTarget::GeneralInfoTarget(QWidget *parent)
    : QWidget{parent}
    , m_frequency(new QLineEdit(this))
    , m_channelData(new QComboBox(this))
    , m_polarization(new QComboBox(this))
    , m_spacecraftNumber(new QLineEdit(this))
{
    init();

    for (int i = 0; i < 12; i++)
        m_channelData->addItem(QString::number(i + 1), QVariant(i));

    m_polarization->addItem("Правая круговая", QVariant(0));
    m_polarization->addItem("Левая круговая", QVariant(1));
    m_polarization->addItem("Вертикальная", QVariant(2));
    m_polarization->addItem("Горизонтальная", QVariant(3));
    m_polarization->addItem("Линейная +45%", QVariant(4));
    m_polarization->addItem("Линейная -45%", QVariant(5));
}

void GeneralInfoTarget::init()
{
    QGridLayout *gridLayout = new QGridLayout(this);

    setStyleSheet("QLabel { background: transparent; }");

    gridLayout->addWidget(new QLabel("Канала данных"), 0, 0);
    gridLayout->addWidget(m_channelData, 0, 1);
    gridLayout->addWidget(new QLabel("Номер КА"), 0, 2);
    gridLayout->addWidget(m_spacecraftNumber, 0, 3);

    gridLayout->addWidget(new QLabel("Поларизация"), 1, 0);
    gridLayout->addWidget(m_polarization, 1, 1);
    gridLayout->addWidget(new QLabel("Частота, кГц"), 1, 2);
    gridLayout->addWidget(m_frequency, 1, 3);
}

void GeneralInfoTarget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.fillRect(rect(), "#F2F0F0");
}
