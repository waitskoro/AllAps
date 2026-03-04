#include "spectrogramdialog.h"
#include "ui_spectrogramdialog.h"

#include <QDateTime>
#include <QStylePainter>

SpectrogramDialog::SpectrogramDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SpectrogramDialog)
    , m_maxHistory(512)
{
    ui->setupUi(this);

    m_timer.setInterval(50);
    connect(&m_timer, &QTimer::timeout, [this](){
        if (m_spectrogramData.size() != 0) {
            updateImage();
            updateTimeline();
        }
    });

    m_showDates = true;
    m_updateEnabled = true;
    m_dateFormat = "  mm:ss";

    m_timer.start();

    connect(ui->speed, &QSpinBox::valueChanged, [this](int value){
        m_timer.setInterval(value);
    });
}

SpectrogramDialog::~SpectrogramDialog()
{
    delete ui;
}

void SpectrogramDialog::addSpectrum(const QVector<double> &spectrum)
{
    if (!m_updateEnabled || spectrum.isEmpty())
        return;

    m_spectrogramData.append(spectrum);
    m_dateTimestamps.append(QDateTime::currentDateTime());

    int maxHistory = qMin(ui->spectrogram->height(), ui->historySize->value());
    while (m_spectrogramData.size() > maxHistory) {
        m_spectrogramData.removeFirst();
        m_dateTimestamps.removeFirst();
    }
}

void SpectrogramDialog::updateImage()
{
    auto width = ui->spectrogram->width();
    auto height = ui->spectrogram->height();
    int displayHeight = ui->historySize->value();

    if (m_spectrogramData.isEmpty() || width <= 0 || height <= 0 || displayHeight <= 0)
        return;

    auto dataHeight = m_spectrogramData.size();
    int spectrumSize = m_spectrogramData.first().size();
    if (spectrumSize <= 0)
        return;

    QImage image(width, height, QImage::Format_ARGB32);
    image.fill(Qt::black);

    int dataRowsToShow = qMin(dataHeight, displayHeight);
    double heightScale = (double)height / dataRowsToShow;

    for (int y = 0; y < height; y++) {
        int dataRow = y / heightScale;
        if (dataRow < dataHeight) {
            const QVector<double> &spectrumRow = m_spectrogramData.at(dataRow);

            for (int x = 0; x < width; x++) {
                double freqPos = (double)x / (width - 1);
                int index = freqPos * (spectrumSize - 1);
                double value = spectrumRow.at(index);

                image.setPixelColor(x, y, value * ui->minLevel->value());
            }
        }
    }

    if (ui->checkBox->isChecked()) {
        QPainter painter(&image);
        drawGrid(painter);
    }

    ui->spectrogram->setPixmap(QPixmap::fromImage(image));
}

void SpectrogramDialog::updateTimeline()
{
    if (m_spectrogramData.isEmpty() || !m_showDates)
        return;

    int width = ui->timeView->width();
    int height = ui->timeView->height();

    if (width <= 0 || height <= 0)
        return;

    if (m_timelineImage.size() != QSize(width, height)) {
        m_timelineImage = QImage(width, height, QImage::Format_ARGB32);
    }

    m_timelineImage.fill(Qt::black);

    QPainter painter(&m_timelineImage);
    painter.setRenderHint(QPainter::Antialiasing);

    drawDates(painter);

    ui->timeView->setPixmap(QPixmap::fromImage(m_timelineImage));
}

void SpectrogramDialog::drawGrid(QPainter &painter)
{
    painter.save();

    QColor color = "white";

    if (ui->comboBoxColor->currentText() == "Красный") {
        color = Qt::red;
    } else if (ui->comboBoxColor->currentText() == "Зеленый") {
        color = Qt::green;
    } else if (ui->comboBoxColor->currentText() == "Синий") {
        color = Qt::blue;
    }

    QPen gridPen(color);
    gridPen.setWidth(ui->spinBoxWidht->value());
    gridPen.setStyle(Qt::DashDotDotLine);
    painter.setPen(gridPen);

    QRect rect = painter.window();
    if (rect.isEmpty())
        rect = painter.viewport();

    int numVerticalLines = ui->spinBoxVertLines->value();
    for (int i = 1; i < numVerticalLines; i++) {
        int x = rect.left() + (i * rect.width()) / numVerticalLines;
        painter.drawLine(x, rect.top(), x, rect.bottom());
    }

    int numHorizontalLines = ui->spinBoxHorizontLines->value();
    for (int i = 1; i < numHorizontalLines; i++) {
        int y = rect.top() + (i * rect.height()) / numHorizontalLines;
        painter.drawLine(rect.left(), y, rect.right(), y);
    }

    painter.drawRect(rect);
    painter.restore();
}

void SpectrogramDialog::drawDates(QPainter& painter)
{
    auto viewHeight = ui->timeView->height();
    auto historyValue = ui->historySize->value();

    int spectrogramHeight = ui->spectrogram->height();
    int dataRowsToShow = qMin(m_spectrogramData.size(), historyValue);

    if (spectrogramHeight <= 0 || dataRowsToShow <= 0)
        return;

    double scale = (double)viewHeight / spectrogramHeight;

    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);

    int firstDatePos = 20;   // отступ сверху для первой даты
    int pixelInterval = 100; // пикселей между датами в спектрограмме

    for (int y = firstDatePos; y < spectrogramHeight && y < dataRowsToShow * (spectrogramHeight / dataRowsToShow);
         y += pixelInterval) {

        int dataIndex = y * dataRowsToShow / spectrogramHeight;

        if (dataIndex < m_dateTimestamps.size()) {
            int timelineY = y * scale;

            QString dateStr;
            if (dataIndex < m_dateTimestamps.size()) {
                dateStr = m_dateTimestamps[dataIndex].toString(m_dateFormat);
            }

            QRect textRect(0, timelineY - 10, 90, 20);
            painter.fillRect(textRect, QColor(0, 0, 0, 180));
            painter.setPen(Qt::white);
            painter.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, dateStr);
        }
    }
}

void SpectrogramDialog::setShowDates(bool show)
{
    m_showDates = show;
    updateTimeline();
}

void SpectrogramDialog::setDateFormat(const QString& format)
{
    m_dateFormat = format;
    updateTimeline();
}

void SpectrogramDialog::resizeEvent(QResizeEvent* event)
{
    QDialog::resizeEvent(event);
    updateImage();
    updateTimeline();
}

QImage SpectrogramDialog::getTimelineImage() const
{
    return m_timelineImage;
}
