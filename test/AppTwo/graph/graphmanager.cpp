#include "graphmanager.h"

#include "ui_mainwindow.h"

#include "dsp.h"
#include "dftplotter.h"
#include "timeplotter.h"
#include "powerplotter.h"

GraphManager::GraphManager(QObject *parent)
    : QObject(parent)
    , m_dsp(new Dsp(this))
{}

void GraphManager::setUi(Ui::MainWindow *ui)
{
    m_ui = ui;

    m_plotterI = new TimePlotter("I", ui->plotterI, this);
    m_plotterQ = new TimePlotter("Q", ui->plotterQ, this);
    m_powerPlotter = new PowerPlotter(ui->plotterPower, this);
    m_dftPlotter = new DftPlotter(m_ui->plotterSpector, this);

    for (int i = 1; i <= 12; i++) {
        m_ui->channel->addItem(QString::number(i));
    }

    connect(m_ui->stopI, &QPushButton::clicked, [this]() {
        m_plotterI->autoRescaleEnable();
    });

    connect(m_ui->stopQ, &QPushButton::clicked, [this]() {
        m_plotterQ->autoRescaleEnable();
    });

    connect(m_ui->stopPower, &QPushButton::clicked, [this]() {
        m_powerPlotter->autoRescaleEnable();
    });

    connect(m_ui->stopSpectrum, &QPushButton::clicked, [this]() {
        m_dftPlotter->autoRescaleEnable();
    });
}

void GraphManager::onSamplesReaded(QVector<std::complex<double> > dataComplex)
{
    if (dataComplex.size() == 0)
        return;

    m_dsp->input(dataComplex,1./110.e3);

    if (m_powerPlotter->isRescale())
        m_powerPlotter->addData(m_dsp->powerFreqDb());

    if (m_plotterI->isRescale())
        m_plotterI->setData(m_dsp->timeVector(), m_dsp->i());

    if (m_plotterQ->isRescale())
        m_plotterQ->setData(m_dsp->timeVector(), m_dsp->q());

    if (m_dftPlotter->isRescale())
        m_dftPlotter->setData(m_dsp->freqVector(), m_dsp->ampDistDb());
}
