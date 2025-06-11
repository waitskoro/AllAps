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

    void appendChannelData(int channelNumber, int iQuadrature, int qQuadrature);

private:
    QThread *m_thread;

    QFile* m_files[CHANNEL_COUNT];

    void write(QString, QFile *__restrict);
    void removeFirstNLines(QFile* file, int n);
};

}
