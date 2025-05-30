#pragma once

#include <QByteArray>
#include <QDataStream>
#include <vector>

struct DesignationTarget {
    qint16 coord[2];

    friend QDataStream &operator>>(QDataStream &stream, DesignationTarget &target) {
        stream >> target.coord[0] >> target.coord[1];
        return stream;
    }
};

struct DataChannelSegment {
    quint8 sectorNumber;
    quint8 physicalChannelNumber;
    quint8 polarizationDirection;
    quint16 spacecraftNumber;
    quint32 centerFrequency;
    double startTime;
    double endTime;
    quint16 targetCount;
    std::vector<DesignationTarget> targets;

    friend QDataStream &operator>>(QDataStream &stream, DataChannelSegment &segment) {
        stream >> segment.sectorNumber
            >> segment.physicalChannelNumber
            >> segment.polarizationDirection
            >> segment.spacecraftNumber
            >> segment.centerFrequency
            >> segment.startTime
            >> segment.endTime
            >> segment.targetCount;

        segment.targets.resize(segment.targetCount);
        for (quint16 i = 0; i < segment.targetCount; ++i) {
            stream >> segment.targets[i];
        }
        return stream;
    }
};

struct DataChannelInfo {
    quint8 channelNumber = 0;
    quint8 segmentCount = 0;
    std::vector<DataChannelSegment> segments;

    friend QDataStream &operator>>(QDataStream &stream, DataChannelInfo &channel) {
        stream >> channel.channelNumber
            >> channel.segmentCount;

        channel.segments.resize(channel.segmentCount);
        for (quint8 i = 0; i < channel.segmentCount; ++i) {
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
        for (quint8 i = 0; i < msg.activeChannelsCount; ++i) {
            stream >> msg.channels[i];
        }
        return stream;
    }
};
