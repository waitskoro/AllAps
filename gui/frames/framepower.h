#pragma once

#include <QFrame>

namespace Ui {
class FramePower;
}

class PowerPlotter;

class FramePower : public QFrame
{
    Q_OBJECT

public:
    explicit FramePower(QWidget *parent = nullptr);
    ~FramePower();

    void setRangeGraph(int value);
    void addData(int channel, double power);
    void setCurrentChannel(int currentChannel);

private:
    Ui::FramePower *ui;

    int m_currentChannel;

    PowerPlotter *m_plotter;
};

