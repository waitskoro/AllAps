#pragma once

#include <QFrame>
#include <QPushButton>

namespace Ui {
class FrameIQ;
}

class Plotter;

class FrameIQ : public QFrame
{
    Q_OBJECT

public:
    explicit FrameIQ(QFrame *parent = nullptr);
    ~FrameIQ();

    bool getRescaleQ();
    bool getRescaleI();

    void setVisibleWidget(int index, bool visible);

    void setDataI(const QVector<double> &time, const QVector<double> &data);
    void setDataQ(const QVector<double> &time, const QVector<double> &data);

private:
    Ui::FrameIQ *ui;
    Plotter *m_plotterI;
    Plotter *m_plotterQ;
};

