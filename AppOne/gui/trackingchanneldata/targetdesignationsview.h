#pragma once

#include <QWidget>

namespace View {

class TargetDesignationModel;

class TargetDesignationsView : public QWidget
{
    Q_OBJECT
public:
    TargetDesignationsView(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *event);
};

}
