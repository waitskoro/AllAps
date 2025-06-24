#pragma once

#include <array>
#include <QIODevice>
#include <QByteArray>
#include <QDataStream>

class Header {

public:
    uint16_t version = 0xAC01;
    uint8_t msgType;
    uint8_t zero = 0x00;
    double timeCreated;
    uint32_t countBytes;

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

using ComplexSample = std::array<int8_t, 2>;

#include <vector>
#include <array>

struct Report {
    quint8 dataChannelNumber;
    quint8 acState;
    quint16 kaNumber;
    double time;
    std::array<qint16, 2> az;
    quint32 count;
    std::vector<std::array<qint8, 2>> info;

    friend QDataStream &operator>>(QDataStream &stream, Report &report) {
        stream.setByteOrder(QDataStream::LittleEndian);

        stream >> report.dataChannelNumber
            >> report.acState
            >> report.kaNumber
            >> report.time
            >> report.az[0]
            >> report.az[1]
            >> report.count;

        report.info.resize(report.count);

        for (auto &sample : report.info) {
            stream >> sample[0] >> sample[1];
        }

        return stream;
    }
};
