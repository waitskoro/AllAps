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

    double minLevel() { return ui->minLevel->value(); }
    double maxLevel() { return ui->maxLevel->value(); }
    int historySize() { return ui->spectrogramHistory->value(); }

signals:
    void powerRangeChanged(int value);
    void checkGraghIChanged(bool checked);
    void checkGraghQChanged(bool checked);
    void checkGraghPowerChanged(bool checked);
    void checkSpectrogramChanged(bool checked);
    void checkGraghSpectorChanged(bool checked);

    void minLevelChanged(double value);
    void maxLevelChanged(double value);
    void spectrogramHistoryChanged(double value);
    void colorSchemeChanged(const QString &text);

private:
    Ui::FrameSettings *ui;

    void connectSettings();
    void onButtonVisibleClicked();
};

