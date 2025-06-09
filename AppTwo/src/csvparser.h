#pragma once

#include <QFile>
#include <QMutex>
#include <QObject>
#include <QVector>

namespace Reports {

struct ChannelData {
    int channelNumber;
    double iQuadrature;
    double qQuadrature;
};

class CsvParser : public QObject
{
    Q_OBJECT
public:
    explicit CsvParser(QObject *parent = nullptr);

    bool appendChannelData(int channelNumber, double iQuadrature, double qQuadrature);

private:
    FILE *m_file;
};

}
