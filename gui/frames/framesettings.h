#pragma once

#include <QFrame>

namespace Ui {
class FrameSettings;
}

class FrameSettings : public QFrame
{
    Q_OBJECT

public:
    explicit FrameSettings(QWidget *parent = nullptr);
    ~FrameSettings();

    double minLevel();
    double maxLevel();
    int historySize();

signals:
    void checkGraghIChanged(bool checked);
    void checkGraghQChanged(bool checked);
    void checkGraghPowerChanged(bool checked);

    void checkSpectrogramChanged(bool checked);
    void checkGraghSpectorChanged(bool checked);

    void powerRangeChanged(int value);

    void minLevelChanged(double value);
    void maxLevelChanged(double value);
    void spectrogramHistoryChanged(double value);
    void colorSchemeChanged(const QString &text);

private:
    Ui::FrameSettings *ui;

    void connectSettings();
};

