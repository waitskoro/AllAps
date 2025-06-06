#pragma once

#include <QIODevice>

struct Emmiter_state{
    quint8 number;
    quint8 pol;
    quint8 state;
    float signalLevel;

    friend QDataStream &operator >> (QDataStream &stream, Emmiter_state &emState)
    {
        stream >> emState.number
            >> emState.pol
            >> emState.state
            >> emState.signalLevel;
        return stream;
    }
};

struct CamState {
    CamState() {}

    quint8 camState;
    float amperage;
    float temperature;
    quint8 camCount;
    Emmiter_state* emmState;

    friend QDataStream &operator >> (QDataStream &stream, CamState &camState)
    {
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        stream >> camState.camState
               >> camState.amperage
               >> camState.temperature
               >> camState.camCount;

        camState.emmState = new Emmiter_state[camState.camCount];

        for(int i = 0; i < camState.camCount; ++i) {
            stream >> camState.emmState[i];
        }

        return stream;
    }

};

struct CdoMessage {
    CdoMessage() {}

    quint8 state;
    float amperage;
    float voltage;
    float temperature;
    quint8 cdoState;
    quint8 camCount;
    CamState * camInfo;

    friend QDataStream &operator >> (QDataStream &stream, CdoMessage &cdoState)
    {
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        stream >> cdoState.state
               >> cdoState.amperage
               >> cdoState.voltage
               >> cdoState.temperature
               >> cdoState.cdoState
               >> cdoState.camCount;

        cdoState.camInfo = new CamState[cdoState.camCount];

        for (int i = 0; i < cdoState.camCount; ++i) {
            stream >> cdoState.camInfo[i];
        }

        return stream;
    }

};

struct StateMessage {
    quint8 acState;
    quint8 frequencyState;
    quint8 switch1State;
    quint8 switch2State;
    quint8 computerState;
    quint8 sectorCount;

    CdoMessage* cdoState;

    StateMessage() : cdoState(nullptr) {}
    ~StateMessage() { delete[] cdoState; }

    friend QDataStream &operator >> (QDataStream &stream, StateMessage &stateMsg)
    {
        stream.setByteOrder(QDataStream::LittleEndian);
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        stream >> stateMsg.acState
            >> stateMsg.frequencyState
            >> stateMsg.switch1State
            >> stateMsg.switch2State
            >> stateMsg.computerState
            >> stateMsg.sectorCount;

        stateMsg.cdoState = new CdoMessage[stateMsg.sectorCount];

        for(quint8 i = 0; i < stateMsg.sectorCount; ++i) {
            stream >> stateMsg.cdoState[i];
        }

        return stream;
    }
};
