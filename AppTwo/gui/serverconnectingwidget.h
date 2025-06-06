#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

namespace View {

class LineEdit;

class ServerConnectingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ServerConnectingWidget(QWidget *parent = nullptr);

signals:
    void createServer(const int port);

private:
    void paintEvent(QPaintEvent *e) override;

    QLineEdit *m_port;
    QPushButton *m_connectButton;
};

}
