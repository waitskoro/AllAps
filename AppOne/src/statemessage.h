#pragma once

#include <QIODevice>

struct Emmiter_state{
    quint8 number = 1;
    quint8 pol = 1;
    quint8 state = 1;
    float signalLevel = 1;

    friend QDataStream &operator >> (QDataStream &stream, Emmiter_state &emState)
    {
        stream >> emState.number
            >> emState.pol
            >> emState.state
            >> emState.signalLevel;
        return stream;
    }
};

struct CAM_state {
    CAM_state() {}

    quint8 camState = 1;
    float amperage = 1;
    float temperature = 1;
    quint8 emmiterCount = 16;
    Emmiter_state* emmState ;

    friend QDataStream &operator >> (QDataStream &stream, CAM_state &camState)
    {
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        stream << camState.camState
               << camState.amperage
               << camState.temperature
               << camState.emmiterCount;

        for(int i = 0; i < camState.emmiterCount; ++i) {
            stream >> camState.emmState[i];
        }

        return stream;
    }

};

struct CDO_state {
    CDO_state() {}

    quint8 state = 1;
    float amperage = 1;
    float voltage = 1;
    float temperature = 1;
    quint8 cdoState = 1;
    quint8 camCount = 16;
    CAM_state * camInfo;

    friend QDataStream &operator >> (QDataStream &stream, CDO_state &cdoState)
    {
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        stream >> cdoState.state
            >> cdoState.amperage
            >> cdoState.voltage
            >> cdoState.temperature
            >> cdoState.cdoState
            >> cdoState.camCount;

        for (int i = 0; i < cdoState.camCount; ++i) {
            stream >> cdoState.camInfo[i];
        }
        return stream;
    }

};

struct StateMessage {
    StateMessage();

    quint8 acState = 0;
    quint8 frequencyState = 0;
    quint8 switch1State = 0;
    quint8 switch2State = 0;
    quint8 computerState = 0;

    quint8 sectorCount = 4;

    CDO_state* cdo_state;

    QByteArray serializeStruct() {
        QByteArray data;
        QDataStream stream (&data, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

        stream >> acState
            >> frequencyState
            >> switch1State
            >> switch2State
            >> computerState
            >> sectorCount;

        for(int i = 0; i < 4; i++) {
            stream >> cdo_state[i];
        }
        return data;
    }
};
