#pragma once

#include <QObject>
#include <QThread>
#include <QMutex>
#include <memory>
#include <array>

constexpr int CHANNEL_COUNT = 12;

class BinParser : public QObject
{
    Q_OBJECT

public:
    explicit BinParser(QObject *parent = nullptr);
    ~BinParser();

    void appendChannelData(int channelNumber, int iQuadrature, int qQuadrature);
    void appendChannelDataBatch(int channelNumber, const QVector<QPair<int, int>> &data);

private:
    struct DataPoint {
        int iQuadrature;
        int qQuadrature;
    };

    void write(const DataPoint* data, size_t count, FILE* file);
    void removeFirstNPoints(FILE* file, int n);
    bool reopenFile(FILE*& file, const QString& filename);

    std::array<FILE*, CHANNEL_COUNT> m_files;
    std::unique_ptr<QThread> m_thread;
    QMutex m_fileMutex;
};
