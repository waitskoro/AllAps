#pragma once

#include <QUrl>
#include <QObject>

namespace Connection {

struct ConnectionData {
    QString acHost;
    int acPort;
    QUrl url;
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
