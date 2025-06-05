#pragma once

#include <array>
#include <vector>
#include <QDataStream>
#include <QByteArray>

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

struct Count {
    uint8_t channelNumber;
    uint8_t infrastructureStatus;
    uint16_t satelliteNumber;
    double referenceTime;
    std::array<int16_t, 2> angles;
    uint32_t sampleCount;
    std::vector<ComplexSample> samples;

    bool isValid() const {
        return infrastructureStatus == 0;
    }

    bool isTimeSynchronized() const {
        return (infrastructureStatus & 0x01) == 0;
    }

    bool hasNonCriticalFault() const {
        return (infrastructureStatus & 0x02) != 0;
    }

    bool hasCriticalFault() const {
        return (infrastructureStatus & 0x04) != 0;
    }

    friend QDataStream &operator >> (QDataStream &stream, Count &data) {
        stream >> data.channelNumber;
        stream >> data.infrastructureStatus;
        stream >> data.satelliteNumber;
        stream >> data.referenceTime;
        stream >> data.angles[0] >> data.angles[1];
        stream >> data.sampleCount;

        data.samples.resize(data.sampleCount);
        for (uint32_t i = 0; i < data.sampleCount; ++i) {
            stream >> data.samples[i][0] >> data.samples[i][1];
        }

        return stream;
    }
};
