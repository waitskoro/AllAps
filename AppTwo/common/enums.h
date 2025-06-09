#pragma once

#include <QObject>

enum StateConnection {
    Disconnected,
    Connecting,
    Connected
};

enum ReportDelegate {
    ChannelNumber = Qt::UserRole + 1,
    KaNumber,
    AcState,
    Count,
    Time,
    Az_1,
    Az_2,
    Info
};
