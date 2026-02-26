#include "graphmanager.h"

#include "ui_mainwindow.h"

#include "dsp.h"
#include "plotter.h"
#include "powerplotter.h"
#include "markersplotter.h"
#include "gui/frames/spectrogramdialog.h"

GraphManager::GraphManager(QObject *parent)
    : QObject(parent)
    , m_dsp(new Dsp(this))
    , m_spectrogramDialog(new SpectrogramDialog())
{
    m_spectrogramDialog->setAttribute(Qt::WA_QuitOnClose, false);
}

GraphManager::~GraphManager()
{}

void GraphManager::setUi(Ui::MainWindow *ui)
{
    m_ui = ui;

    connect(ui->comboBoxChannel,
            &QComboBox::currentIndexChanged,
            this,
            &GraphManager::onCurrentIndexChanged);

    // =========================================================================

    connect(ui->frameSettings,
            &FrameSettings::checkGraghIChanged,
            [ui] (bool checked) {
                ui->frameIQ->setVisibleWidget(0, checked);
            });

    connect(ui->frameSettings,
            &FrameSettings::checkGraghQChanged,
            [ui](bool checked) {
                ui->frameIQ->setVisibleWidget(1, checked);
            });

    connect(ui->frameSettings,
            &FrameSettings::checkGraghPowerChanged,
            [ui](bool checked) {
                ui->framePower->setVisible(checked);
            });

    connect(ui->frameSettings,
            &FrameSettings::checkGraghSpectorChanged,
            [ui](bool checked) {
                ui->frameSpector->setVisibleWidget(1, checked);
            });

    // =========================================================================

    connect(ui->frameSettings,
            &FrameSettings::powerRangeChanged,
            [ui](int value) {
                ui->framePower->setRangeGraph(value);
            });

    // =========================================================================

    connect(ui->frameSettings, &FrameSettings::spectrogramView,
            m_spectrogramDialog, &SpectrogramDialog::show);
}

void GraphManager::setAzimutAngle(int channel, double azimut, double angle)
{
    if (channel == m_ui->comboBoxChannel->currentIndex() + 1) {
        m_ui->label_azimut->setText(QString::number(azimut, 'f', 1));
        m_ui->label_angle->setText(QString::number(angle, 'f', 1));

        m_azimutAngleOnChannel[channel] = std::make_pair(azimut, angle);
    }
}

void GraphManager::onSamplesReaded(int channel, const QVector<std::complex<double>> &data)
{
    if (data.size() == 0)
        return;

    m_dsp->input(data, 1./110.e3);

    m_ui->framePower->addData(channel, m_dsp->powerFreqDb());

    if (channel != m_ui->comboBoxChannel->currentIndex() + 1)
        return;

    if (m_ui->frameIQ->getRescaleI())
        m_ui->frameIQ->setDataI(m_dsp->timeVector(), m_dsp->i());

    if (m_ui->frameIQ->getRescaleQ())
        m_ui->frameIQ->setDataQ(m_dsp->timeVector(), m_dsp->q());

    if (m_ui->frameSpector->getRescale())
        m_ui->frameSpector->setData(m_dsp->freqVector(), m_dsp->ampDistDb());

    if (m_spectrogramDialog->isVisible())
        m_spectrogramDialog->addSpectrum(m_dsp->ampDistDb());
}

void GraphManager::onCurrentIndexChanged(int index)
{
    m_ui->framePower->setCurrentChannel(index);

    auto it = m_azimutAngleOnChannel.find(index);
    if (it != m_azimutAngleOnChannel.end()) {
        double azimuth = it->first;
        double angle = it->second;

        m_ui->label_azimut->setText(QString::number(azimuth, 'f', 1));
        m_ui->label_angle->setText(QString::number(angle, 'f', 1));
    } else {
        m_ui->label_azimut->setText("0.0");
        m_ui->label_angle->setText("0.0");
    }
}
