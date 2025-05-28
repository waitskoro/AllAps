#pragma once

#include <QObject>

namespace Connection {

struct ConnectionData {
    QString acHost;
    QString acPort;
    QString p2Host;
    QString p2Port;
};

class ConnectionDataSaving : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionDataSaving(QObject *parent = nullptr);

    ConnectionData connectionData();
    void setConnectionData(const QUrl &ac);
private:
    QString m_path;

    QString readFile();
};

}
