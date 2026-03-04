#ifndef SPECTROGRAMDIALOG_H
#define SPECTROGRAMDIALOG_H

#include <QTimer>
#include <QImage>
#include <QThread>
#include <QDialog>
#include <QVector>
#include <QDateTime>
#include <QResizeEvent>

namespace Ui {
class SpectrogramDialog;
}

class SpectrogramDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SpectrogramDialog(QWidget *parent = nullptr);
    ~SpectrogramDialog();

    void addSpectrum(const QVector<double>& spectrum);

    void setShowDates(bool show);
    void setDateFormat(const QString& format);

    QImage getTimelineImage() const;

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void updateImage();
    void updateTimeline();
    void drawGrid(QPainter &painter);
    void drawDates(QPainter& painter);

    QTimer m_timer;

    Ui::SpectrogramDialog *ui;

    QList<QVector<double>> m_spectrogramData;
    QList<QDateTime> m_dateTimestamps;
    QImage m_timelineImage;

    int m_maxHistory;
    bool m_updateEnabled;
    bool m_showDates;
    QString m_dateFormat;
};

#endif // SPECTROGRAMDIALOG_H
