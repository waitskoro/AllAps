#pragma once

#include <QImage>
#include <QTimer>
#include <QColor>
#include <QObject>
#include <QVector>

class Spectrogram : public QObject
{
    Q_OBJECT

public:
    explicit Spectrogram(QObject *parent = nullptr);
    ~Spectrogram();

    void clear();
    QImage getImage() const;
    void setSize(int width, int height);
    void addSpectrum(const QVector<double>& spectrum);

    void setColorMap(const QString& mapName);
    void setMaxLevel(double maxLevel);
    void setMinLevel(double minLevel);
    void setUpdateEnabled(bool enabled) { m_updateEnabled = enabled; }

    int maxHistory() const { return m_maxHistory; }
    int historySize() const { return m_spectrogramData.size(); }
    void setMaxHistory(int max) { m_maxHistory = qMax(1, max); }

signals:
    void imageUpdated(const QImage& image);

private slots:
    void processPendingData();

private:
    void updateImage();
    void updateDynamicRange();
    void createColorMap(const QString& mapName);

    QImage m_image;
    QVector<QVector<double>> m_pendingData;
    QVector<QVector<double>> m_spectrogramData;

    int m_width;
    int m_height;
    int m_maxHistory;
    bool m_updateEnabled;

    double m_minDb;
    double m_maxDb;

    QTimer* m_updateTimer;
    QVector<QColor> m_colorMap;
};
