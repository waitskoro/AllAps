#pragma once

#include <QFile>
#include <QMutex>
#include <QObject>
#include <QVector>

namespace {
const int CHANNEL_COUNT = 12;
}

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
    ~CsvParser();

    bool appendChannelData(int channelNumber, double iQuadrature, double qQuadrature);

private:
    FILE *m_fileCh1;
    FILE *m_fileCh2;
    FILE *m_fileCh3;
    FILE *m_fileCh4;
    FILE *m_fileCh5;
    FILE *m_fileCh6;
    FILE *m_fileCh7;
    FILE *m_fileCh8;
    FILE *m_fileCh9;
    FILE *m_fileCh10;
    FILE *m_fileCh11;
    FILE *m_fileCh12;

    QThread *m_thread;

    FILE* m_files[CHANNEL_COUNT];
    bool write(QString, FILE *__restrict);
};

}
