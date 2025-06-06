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
    Info
};
