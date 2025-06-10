#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

namespace Connection {
struct ConnectionData;
}

namespace View {

class Loader;

class IpConnectionSetupWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IpConnectionSetupWidget(QWidget *parent = nullptr);
    ~IpConnectionSetupWidget();

    void showWindow(Connection::ConnectionData);
    void onPushButtonClicked();

signals:
    void connectToHosts(const QUrl &ac);

private:
    void initUI();
    void updateWidgetPositions();
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);
    void drawLabels(QPainter &painter);

    QRect m_mainRect;
    QLineEdit *m_acIpTextEdit;
    QPushButton *m_pushButton;
    QLineEdit *m_acPortTextEdit;

    QLineEdit* createLineEdit(int width, int height);


    Loader *m_loader;
};

}
