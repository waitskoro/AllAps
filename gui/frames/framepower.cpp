#include "framepower.h"
#include "ui_framepower.h"

#include "graph/powerplotter.h"

FramePower::FramePower(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::FramePower)
{
    ui->setupUi(this);
    m_plotter = new PowerPlotter(ui->plotterPower, this);

    connect(ui->buttonClear, &QPushButton::clicked, [this] {
        m_plotter->clearData(m_currentChannel);
    });
}

FramePower::~FramePower()
{
    delete ui;
    delete m_plotter;
}

void FramePower::setRangeGraph(int value)
{
    m_plotter->setRangeGraph(value);
}

void FramePower::addData(int channel, double power)
{
    m_plotter->addData(channel , power);
}

void FramePower::setCurrentChannel(int currentChannel)
{
    m_currentChannel = currentChannel + 1;
}
