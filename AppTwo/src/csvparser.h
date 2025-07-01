#pragma once

#include <QObject>
#include <QFile>
#include <QThread>
#include <QMutex>
#include <QTextStream>
#include <memory>
#include <array>

namespace Reports {

constexpr int CHANNEL_COUNT = 12;

class CsvParser : public QObject
{
    Q_OBJECT

public:
    explicit CsvParser(QObject *parent = nullptr);
    ~CsvParser();

    void appendChannelData(int channelNumber, int iQuadrature, int qQuadrature);
    void appendChannelDataBatch(int channelNumber, const QVector<QPair<int, int>> &data);

private:
    void write(const QString &str, QFile* file);
    void removeFirstNLines(QFile *file, int n);
    bool reopenFile(QFile* file);

    std::array<std::unique_ptr<QFile>, CHANNEL_COUNT> m_files;
    std::unique_ptr<QThread> m_thread;
    QMutex m_fileMutex;
};

} // namespace Reports
