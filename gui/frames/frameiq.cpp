#include "frameiq.h"
#include "ui_frameiq.h"

#include "graph/plotter.h"
#include "commonfunctions.h"

FrameIQ::FrameIQ(QFrame *parent)
    : QFrame(parent)
    , ui(new Ui::FrameIQ)
{
    ui->setupUi(this);
    m_plotterI = new Plotter(ui->plotterI, this);
    m_plotterQ = new Plotter(ui->plotterQ, this);

    connect(ui->buttonStopI, &QPushButton::clicked, [this] {
        handleStopButton(ui->buttonStopI, m_plotterI);
    });

    connect(ui->buttonStopQ, &QPushButton::clicked, [this] {
        handleStopButton(ui->buttonStopQ, m_plotterQ);
    });
}

FrameIQ::~FrameIQ()
{
    delete ui;
    delete m_plotterI;
    delete m_plotterQ;
}

bool FrameIQ::getRescaleQ()
{
    return m_plotterQ->getRescale();
}

bool FrameIQ::getRescaleI()
{
    return m_plotterI->getRescale();
}

void FrameIQ::setVisibleWidget(int index, bool visible)
{
    ui->splitter->widget(index)->setVisible(visible);

    if (visible && !isVisible()) setVisible(visible);

    if (!ui->splitter->widget(0)->isVisible() &&
        !ui->splitter->widget(1)->isVisible()) {
        setVisible(false);
    }
}

void FrameIQ::setDataI(const QVector<double> &time, const QVector<double> &data)
{
    m_plotterI->setData(time, data);
}

void FrameIQ::setDataQ(const QVector<double> &time, const QVector<double> &data)
{
    m_plotterQ->setData(time, data);
}
