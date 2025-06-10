#pragma once

#include <QObject>

namespace View {
class IpConnectionSetupWidget;
}

namespace Connection {

class ConnectionDataSaving;

class IpConnectionSetupManager : public QObject
{
    Q_OBJECT
public:
    explicit IpConnectionSetupManager(QObject *parent = nullptr);

    void open();
    void close();
signals:
    void connectToHosts(const QUrl &ac);

private:
    ConnectionDataSaving *m_connectionData;
    View::IpConnectionSetupWidget *m_connectionWidget;
};

}
