#pragma once

#include <QFrame>

#include "ui_framesettings.h"

namespace Ui {
class FrameSettings;
}

class FrameSettings : public QFrame
{
    Q_OBJECT

public:
    explicit FrameSettings(QWidget *parent = nullptr);
    ~FrameSettings();

signals:
    void powerRangeChanged(int value);
    void checkGraghIChanged(bool checked);
    void checkGraghQChanged(bool checked);
    void checkGraghPowerChanged(bool checked);
    void checkGraghSpectorChanged(bool checked);

    void spectrogramView();

private:
    Ui::FrameSettings *ui;

    void connectSettings();
    void onButtonVisibleClicked();
};

