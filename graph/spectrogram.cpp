#include "spectrogram.h"
#include <QPainter>
#include <QDebug>
#include <cmath>

Spectrogram::Spectrogram(QObject *parent)
    : QObject(parent)
    , m_width(800)
    , m_height(400)
    , m_maxHistory(100)
    , m_updateEnabled(true)
    , m_minDb(-120)
    , m_maxDb(0)
{
    createColorMap("Jet");

    m_image = QImage(m_width, m_height, QImage::Format_ARGB32);
    m_image.fill(Qt::black);

    m_updateTimer = new QTimer(this);
    m_updateTimer->setInterval(100);
    connect(m_updateTimer, &QTimer::timeout, this, &Spectrogram::processPendingData);
    m_updateTimer->start();
}

Spectrogram::~Spectrogram()
{
    m_updateTimer->stop();
}

void Spectrogram::setSize(int width, int height)
{
    if (width <= 0 || height <= 0)
        return;

    m_width = width;
    m_height = height;

    m_image = QImage(m_width, m_height, QImage::Format_ARGB32);
    m_image.fill(Qt::black);

    m_spectrogramData.clear();
    m_pendingData.clear();
}

void Spectrogram::addSpectrum(const QVector<double>& spectrum)
{
    if (!m_updateEnabled || spectrum.isEmpty())
        return;

    m_pendingData.append(spectrum);

    while (m_pendingData.size() > 5) {
        m_pendingData.removeFirst();
    }
}

void Spectrogram::processPendingData()
{
    if (m_pendingData.isEmpty())
        return;

    for (const auto& spectrum : m_pendingData) {
        if (!spectrum.isEmpty()) {
            m_spectrogramData.prepend(spectrum);
        }
    }

    while (m_spectrogramData.size() > m_maxHistory) {
        m_spectrogramData.removeLast();
    }

    m_pendingData.clear();

    updateImage();
    emit imageUpdated(m_image);
}

void Spectrogram::updateImage()
{
    if (m_spectrogramData.isEmpty() || m_width <= 0 || m_height <= 0)
        return;

    QImage tempImage(m_width, m_height, QImage::Format_ARGB32);
    tempImage.fill(Qt::black);

    int dataHeight = qMin(m_spectrogramData.size(), m_height);
    int spectrumSize = m_spectrogramData.first().size();

    if (spectrumSize <= 0) return;

    for (int y = 0; y < dataHeight; y++) {
        if (y >= m_spectrogramData.size()) break;

        const QVector<double>& spectrum = m_spectrogramData[y];

        for (int x = 0; x < m_width; x++) {
            double freqPos = (double)x / (m_width - 1);
            int idx = freqPos * (spectrumSize - 1);
            idx = qBound(0, idx, spectrumSize - 1);

            double value = spectrum[idx];

            double normalized = (value - m_minDb) / (m_maxDb - m_minDb);
            normalized = qBound(0.0, normalized, 1.0);

            int colorIdx = normalized * (m_colorMap.size() - 1);
            colorIdx = qBound(0, colorIdx, m_colorMap.size() - 1);

            tempImage.setPixelColor(x, m_height - y - 1, m_colorMap[colorIdx]);
        }
    }

    m_image = tempImage;
}

void Spectrogram::updateDynamicRange()
{
    if (!m_spectrogramData.isEmpty()) {
        updateImage();
        emit imageUpdated(m_image);
    }
}

void Spectrogram::createColorMap(const QString& mapName)
{
    m_colorMap.clear();

    if (mapName == "Jet") {
        for (int i = 0; i < 256; i++) {
            int r, g, b;

            if (i < 32) {
                r = 0;
                g = 0;
                b = 127 + i * 4;
            } else if (i < 96) {
                r = 0;
                g = (i - 32) * 4;
                b = 255;
            } else if (i < 160) {
                r = (i - 96) * 4;
                g = 255;
                b = 255 - (i - 96) * 4;
            } else if (i < 224) {
                r = 255;
                g = 255 - (i - 160) * 4;
                b = 0;
            } else {
                r = 255 - (i - 224) * 4;
                g = 0;
                b = 0;
            }

            m_colorMap.append(QColor(r, g, b));
        }
    } else if (mapName == "Hot") {
        for (int i = 0; i < 256; i++) {
            int r = i < 96 ? i * 2.66 : 255;
            int g = i < 192 ? qMax(0, (i - 64) * 2) : 255;
            int b = qMax(0, (i - 192) * 4);

            m_colorMap.append(QColor(r, g, b));
        }
    } else if (mapName == "Gray") {
        for (int i = 0; i < 256; i++) {
            m_colorMap.append(QColor(i, i, i));
        }
    } else {
        for (int i = 0; i < 256; i++) {
            int hue = i * 300 / 256;
            m_colorMap.append(QColor::fromHsv(hue, 255, 255));
        }
    }
}

QImage Spectrogram::getImage() const
{
    return m_image;
}

void Spectrogram::clear()
{
    m_spectrogramData.clear();
    m_pendingData.clear();
    m_image.fill(Qt::black);
    emit imageUpdated(m_image);
}

void Spectrogram::setMaxLevel(double maxLevel)
{
    m_maxDb = maxLevel;
    updateDynamicRange();
}

void Spectrogram::setMinLevel(double minLevel)
{
    m_minDb = minLevel;
    updateDynamicRange();
}

void Spectrogram::setColorMap(const QString& mapName)
{
    createColorMap(mapName);

    if (!m_spectrogramData.isEmpty()) {
        updateImage();
        emit imageUpdated(m_image);
    }
}
