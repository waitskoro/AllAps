#include "framespector.h"
#include "ui_framespector.h"

#include "commonfunctions.h"
#include "graph/spectrogram.h"
#include "graph/markersplotter.h"

FrameSpector::FrameSpector(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::FrameSpector)
    , m_spectrogram(new Spectrogram(this))
    , m_model(new QStandardItemModel(this))
{
    ui->setupUi(this);
    ui->listViewMarkers->setModel(m_model);
    m_plotter = new MarkersPlotter(ui->plotter, this);

    connect(ui->buttonStop, &QPushButton::clicked, [this] {
       handleStopButton(ui->buttonStop, m_plotter);
    });

    connect(ui->buttonClearMarkers, &QPushButton::clicked, [this] {
        m_model->clear();
        m_plotter->clearMarkers();
    });

    connect(m_plotter, &MarkersPlotter::markerChanged,
            this, &FrameSpector::onMarkerChanged);

    connect(m_spectrogram, &Spectrogram::imageUpdated,
            this, &FrameSpector::updateSpectrogram);

    m_spectrogram->setSize(ui->labelSpectrogram->width(),
                           ui->labelSpectrogram->height());

    QImage blackImage(ui->labelSpectrogram->width(),
                      ui->labelSpectrogram->height(),
                      QImage::Format_ARGB32);
    blackImage.fill(Qt::black);
    QPixmap blackPixmap = QPixmap::fromImage(blackImage);
    ui->labelSpectrogram->setPixmap(blackPixmap);
}

FrameSpector::~FrameSpector()
{
    delete ui;
}

bool FrameSpector::getRescale()
{
    return m_plotter->getRescale();
}

void FrameSpector::setHistorySize(int value)
{
    m_spectrogram->setMaxHistory(value);
}

void FrameSpector::setMinLevel(double value)
{
    m_spectrogram->setMinLevel(value);
}

void FrameSpector::setMaxLevel(double value)
{
    m_spectrogram->setMaxLevel(value);
}

void FrameSpector::setColorScheme(const QString &text)
{
    m_spectrogram->setColorMap(text);
}

void FrameSpector::addSpectrum(const QVector<double>& spectrum)
{
    m_spectrogram->addSpectrum(spectrum);
}

void FrameSpector::setVisibleWidget(int index, bool visible)
{
    ui->splitter->widget(index)->setVisible(visible);

    if (visible) setVisible(true);

    if (!ui->splitter->widget(0)->isVisible() &&
        !ui->splitter->widget(1)->isVisible()) {
        setVisible(false);
    }
}

void FrameSpector::setData(const QVector<double> &freq,
                           const QVector<double> &ampDist)
{
    m_plotter->setData(freq, ampDist);
}

void FrameSpector::updateSpectrogram()
{
    QImage image = m_spectrogram->getImage();
    QImage scaledImage = image.scaled(ui->labelSpectrogram->size(),
                                      Qt::IgnoreAspectRatio,
                                      Qt::SmoothTransformation);

    ui->labelSpectrogram->setPixmap(QPixmap::fromImage(scaledImage));
}

void FrameSpector::onMarkerChanged(int index, QString value)
{
    auto text = QString("№%1 %2").arg(index + 1).arg(value);

    if (m_model->rowCount() < index) {
        m_model->appendRow(new QStandardItem(text));
    } else {
        QStandardItem *retrievedItem = m_model->item(index, 0);
        if (retrievedItem)
            retrievedItem->setText(text);
    }
}
