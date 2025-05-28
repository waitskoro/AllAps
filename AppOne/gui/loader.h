#pragma once

#include <QMovie>
#include <QLabel>
#include <QWidget>
#include <QPushButton>

namespace View {

class Loader : public QWidget
{
    Q_OBJECT

public:
    explicit Loader(const QString &text,
                    const QRect &rect,
                    QWidget *parent = nullptr);
    void start();
    void stop();

signals:
    void cancelConnecting();

private slots:
    void onCanceling();

private:
    QMovie* m_movie;
    QRect m_mainRect;
    QRect m_rect;
    QPushButton* m_cancel;

    QString m_headerText;

    int m_dotCount;
    QTimer *m_dotsTimer;

    QString m_baseText;

    void updateDots();
    void paintEvent(QPaintEvent *e) override;
};

}
