#pragma once

#include <array>
#include <QVector>
#include <QtGlobal>
#include <QByteArray>
#include <QDataStream>

class Header {

public:
    quint16 version = 0xAC01;
    quint8 msgType;
    quint8 zero = 0x00;
    double timeCreated;
    quint32 countBytes;

    QByteArray serializeStruct() const
    {
        QByteArray data;
        QDataStream stream(&data, QDataStream::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream << version
               << msgType
               << zero
               << timeCreated
               << countBytes;

        return data;
    }
};

class Packet {

public:
    Packet() {}

    Packet(const Header& header,
           const QByteArray& data,
           long long id)
        : header{header}
        , data{data}
        , id{id}
    {}

    Header header;
    QByteArray data;
    long long id;
};

struct Report {
    quint8 channel;
    quint8 acState;
    quint16 kaNumber;
    double time;
    std::array<qint16, 2> az;
    quint32 count;
    bool is16;
    QVector<std::array<qint8, 2>> info_8;
    QVector<std::array<qint16, 2>> info_16;


    friend QDataStream &operator>>(QDataStream &stream, Report &report) {
        stream.setByteOrder(QDataStream::LittleEndian);

        stream >> report.channel
            >> report.acState
            >> report.kaNumber
            >> report.time
            >> report.az[0]
            >> report.az[1]
            >> report.count;

        if (report.is16) {
            report.info_16.resize(report.count);
            for (auto &sample : report.info_16) {
                stream >> sample[0] >> sample[1];
            }

        } else {
            report.info_8.resize(report.count);
            for (auto &sample : report.info_8) {
                stream >> sample[0] >> sample[1];
            }
        }

        return stream;
    }
};
