#pragma once

enum StateConnection {
    Disconnected,
    Connecting,
    Connected
};

enum ReportDelegate {
    ChannelNumber,
    KaNumber,
    AcState,
    Count,
    Time,
    Az_1,
    Az_2,
    Info
};
