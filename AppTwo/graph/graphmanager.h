#pragma once

#include <QMap>
#include <QObject>
#include <complex>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class Dsp;
class Plotter;
class PowerPlotter;
class MarkersPlotter;
class SpectrogramDialog;

class GraphManager : public QObject
{
    Q_OBJECT
public:
    explicit GraphManager(QObject *parent = nullptr);
    ~GraphManager();

    void setUi(Ui::MainWindow *ui);
    void setAzimutAngle(int channel, double azimut, double angle);
    void onSamplesReaded(int channel, const QVector<std::complex<double>> &data);

private:
    QTimer *timer;

    Dsp *m_dsp;
    Ui::MainWindow *m_ui;
    SpectrogramDialog *m_spectrogramDialog;

    QMap<int, std::pair<double, double>> m_azimutAngleOnChannel;

    void onCurrentIndexChanged(int index);
};

