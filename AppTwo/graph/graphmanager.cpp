#include "graphmanager.h"

#include "ui_mainwindow.h"

#include "dsp.h"
#include "plotter.h"
#include "powerplotter.h"
#include "markersplotter.h"

GraphManager::GraphManager(QObject *parent)
    : QObject(parent)
    , m_dsp(new Dsp(this))
{}

void GraphManager::setUi(Ui::MainWindow *ui)
{
    m_ui = ui;

    m_plotterI = new Plotter(m_ui->plotterI, this);
    m_plotterQ = new Plotter(m_ui->plotterQ, this);
    m_dftPlotter = new MarkersPlotter(m_ui->plotterSpector, this);
    m_powerPlotter = new PowerPlotter(m_ui->plotterPower, this);

    m_dftPlotter->setUi(ui);
    // Отображение маркеров на графике спектра
    m_dftPlotter->setMarkersUsage(true);

    connect(m_ui->stopI, &QPushButton::clicked,
            [this]() { handleStopButton(m_ui->stopI, m_plotterI); });
    connect(m_ui->stopQ, &QPushButton::clicked,
            [this]() { handleStopButton(m_ui->stopQ, m_plotterQ); });
    connect(m_ui->stopPower, &QPushButton::clicked,
            [this]() { handleStopButton(m_ui->stopPower, m_powerPlotter); });
    connect(m_ui->stopSpectrum, &QPushButton::clicked,
            [this]() { handleStopButton(m_ui->stopSpectrum, m_dftPlotter); });

    connect(m_ui->pushButtonPwrClean, &QPushButton::clicked, [this, ui] () {
        m_powerPlotter->clearData(ui->comboBoxChannel->currentIndex() + 1);
    });

    connect(ui->comboBoxChannel, &QComboBox::currentIndexChanged, [this, ui]() {
        int channel = ui->comboBoxChannel->currentIndex() + 1;

        m_powerPlotter->setCurrentChannel(channel);

        auto it = m_azimutAngleOnChannel.find(channel);
        if (it != m_azimutAngleOnChannel.end()) {
            double azimuth = it->first;
            double angle = it->second;

            m_ui->label_azimut->setText(QString::number(azimuth, 'f', 1));
            m_ui->label_angle->setText(QString::number(angle, 'f', 1));
        } else {
            m_ui->label_azimut->setText("0.0");
            m_ui->label_angle->setText("0.0");
        }
    });

    connect(ui->spinBoxRange, &QSpinBox::valueChanged, [this] (int i) {
        m_powerPlotter->setRangeGraph(i);
    });
}

void GraphManager::setAzimutAngle(int channel, double azimut, double angle)
{
    if (channel == m_ui->comboBoxChannel->currentIndex() + 1) {
        m_ui->label_azimut->setText(QString::number(azimut, 'f', 1));
        m_ui->label_angle->setText(QString::number(angle, 'f', 1));

        m_azimutAngleOnChannel[channel] = std::make_pair(azimut, angle);
    }
}

void GraphManager::handleStopButton(QPushButton* button, Plotter* plotter) {
    plotter->setRescale();
    bool isRescale = plotter->getRescale();
    button->setText(isRescale ? "Стоп" : "Запуск");
}

void GraphManager::onSamplesReaded(int channel, QVector<std::complex<double>> dataComplex)
{
    if (dataComplex.size() == 0)
        return;

    m_dsp->input(dataComplex, 1./110.e3);

    m_powerPlotter->addData(channel, m_dsp->powerFreqDb());

    if (channel != m_ui->comboBoxChannel->currentIndex() + 1)
        return;

    if (m_plotterI->getRescale())
        m_plotterI->setData(m_dsp->timeVector(), m_dsp->i());

    if (m_plotterQ->getRescale())
        m_plotterQ->setData(m_dsp->timeVector(), m_dsp->q());

    if (m_dftPlotter->getRescale())
        m_dftPlotter->setData(m_dsp->freqVector(), m_dsp->ampDistDb());
}
