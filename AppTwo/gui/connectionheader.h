#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

namespace View {

class ConnectionHeaderBlock;

class ConnectionHeader : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionHeader(QWidget *parent = nullptr);

    void onConnected();
    void onDisconnected();

signals:
    void disconnect();
    void connectTo(const QString &ip, const QString &port);

private:
    void paintEvent(QPaintEvent *e) override;

    QPushButton *m_connectBtn;

    QLineEdit *m_host;
    QLineEdit *m_port;

    ConnectionHeaderBlock *m_block;
};

}
