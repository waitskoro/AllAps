#pragma once

#include <QFile>
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

    QVector<ChannelData> parseChannelData();
    bool appendChannelData(int channelNumber, double iQuadrature, double qQuadrature);

private:
    QFile m_file;
};

} // namespace Reports
