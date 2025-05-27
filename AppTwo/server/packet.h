#pragma once

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
        stream << version << msgType << zero << timeCreated << countBytes;

        return data;
    }
};

class Packet {
public:
    Packet()
    {}

    Packet(const Header& header,  const QByteArray& data, long long id) :
        header{header}, data{data}, id{id}
    {}

    Header header;
    QByteArray data;
    long long id;
};
