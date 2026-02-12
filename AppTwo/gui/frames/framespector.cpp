#include "framespector.h"
#include "ui_framespector.h"

#include "commonfunctions.h"

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

void FrameSpector::setVisibleWidget(int index, bool visible)
{
    ui->splitter->widget(index)->setVisible(visible);

    if (visible) setVisible(true);

    if (!ui->splitter->widget(0)->isVisible() &&
        !ui->splitter->widget(1)->isVisible()) {
        setVisible(false);
    }
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
