#include "connectiondatasaving.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

using namespace Connection;

ConnectionDataSaving::ConnectionDataSaving(QObject *parent)
    : QObject(parent)
{
    m_path = "../connectionParameter.json";
}

ConnectionData ConnectionDataSaving::connectionData()
{
    auto result = readFile();

    if (result.isEmpty() || result.isNull())
        return {};

    QJsonDocument doc = QJsonDocument::fromJson(result.toUtf8());
    QJsonObject root = doc.object();

    QString acHost = root.value("acHost").toString();
    QString acPort = QString::number(root.value("acPort").toInt());

    return {acHost, acPort};
}

void ConnectionDataSaving::setConnectionData(const QUrl &ac)
{
    QFile file(m_path);
    file.open(QIODevice::ReadWrite);

    if(file.isOpen()){
        file.resize(0);

        QJsonObject obj;
        obj["acHost"] = ac.host();
        obj["acPort"] = ac.port();

        QJsonDocument doc;
        doc.setObject(obj);
        QByteArray dataJs = doc.toJson();

        file.write(dataJs);
        file.close();
    }
}

QString ConnectionDataSaving::readFile()
{
    QString result;
    QFile file;
    file.setFileName(m_path);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    if(file.isOpen()){
        result = file.readAll();
        file.close();
        return result;
    }
    return "";
}
