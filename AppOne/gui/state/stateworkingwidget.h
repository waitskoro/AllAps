#pragma once

#include <QLabel>
#include <QWidget>
#include <QPushButton>

namespace View {

class StateWorkingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StateWorkingWidget(QWidget *parent = nullptr);

    void initUI();

signals:
    void exit();

private:
    QLabel *m_label;
    QPushButton *m_exit;
    QLabel *m_headerLabel;

    void paintEvent(QPaintEvent *event);
};

}
