#pragma once

#include <QByteArray>
#include <QDataStream>
#include <vector>
#include <QDebug>

struct DataChannelSegment {
    uint8_t sectorNumber;
    uint8_t physicalChannelNumber;
    uint8_t polarizationDirection;
    uint16_t spacecraftNumber;
    uint32_t centerFrequency;
    double startTime;
    double endTime;
    uint16_t targetCount;
    int16_t** targets;

    friend QDataStream &operator >> (QDataStream &stream, DataChannelSegment &segment) {
        stream >> segment.sectorNumber
               >> segment.physicalChannelNumber
               >> segment.polarizationDirection
               >> segment.spacecraftNumber
               >> segment.centerFrequency
               >> segment.startTime
               >> segment.endTime
               >> segment.targetCount;

        segment.targets = new int16_t*[segment.targetCount];

        for (int i = 0; i < segment.targetCount; i++) {
            segment.targets[i] = new int16_t[2];
        }

        for (int i = 0; i < segment.targetCount; ++i) {
            stream >> segment.targets[i][0]
                   >> segment.targets[i][1];
        }
        return stream;
    }
};

struct DataChannelInfo {
    uint8_t channelNumber = 0;
    uint8_t segmentCount = 0;
    std::vector<DataChannelSegment> segments;

    friend QDataStream &operator >> (QDataStream &stream, DataChannelInfo &channel) {
        stream >> channel.channelNumber
               >> channel.segmentCount;

        channel.segments.resize(channel.segmentCount);
        for (int i = 0; i < channel.segmentCount; i++) {
            stream >> channel.segments[i];
        }
        return stream;
    }
};

struct DataChannelMessage {
    quint8 activeChannelsCount;
    std::vector<DataChannelInfo> channels;

    friend QDataStream &operator>>(QDataStream &stream, DataChannelMessage &msg) {
        stream.setByteOrder(QDataStream::LittleEndian);
        stream >> msg.activeChannelsCount;

        msg.channels.resize(msg.activeChannelsCount);
        for (int i = 0; i < msg.activeChannelsCount; i++) {
            stream >> msg.channels[i];
        }
        return stream;
    }
};
