#pragma once

#include <QWidget>

namespace Ui {
class TargetDesignationsTest;
}

class TargetDesignationsTest : public QWidget
{
    Q_OBJECT

public:
    explicit TargetDesignationsTest(QWidget *parent = nullptr);
    ~TargetDesignationsTest();

private:
    Ui::TargetDesignationsTest *ui;
};

