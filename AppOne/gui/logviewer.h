#pragma once

#include <QWidget>

namespace View {

class LogViewer : public QWidget
{
    Q_OBJECT
public:
    explicit LogViewer(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *e);
};

}
