#include "csvparser.h"
#include <QTemporaryFile>
#include <QDebug>

using namespace Reports;

CsvParser::CsvParser(QObject *parent)
    : QObject(parent)
    , m_thread(std::make_unique<QThread>())
{
    moveToThread(m_thread.get());
    m_thread->start();

    for (int i = 0; i < CHANNEL_COUNT; ++i) {
        QString filename = QString("../reports_%1.csv").arg(i+1);
        auto file = std::make_unique<QFile>(filename);

        if (!file->open(QIODevice::Append | QIODevice::Text | QIODevice::ReadWrite)) {
            qWarning() << "Failed to open file:" << filename;
            continue;
        }

        m_files[i] = std::move(file);
    }
}

CsvParser::~CsvParser()
{
    m_thread->requestInterruption();
    m_thread->quit();

    if (!m_thread->wait(1000)) {
        qWarning() << "Forcing thread termination";
        m_thread->terminate();
    }

    QMutexLocker locker(&m_fileMutex);
    for (auto& file : m_files) {
        if (file && file->isOpen()) {
            file->close();
        }
    }
}

void CsvParser::appendChannelData(int channelNumber, int iQuadrature, int qQuadrature)
{
    if (channelNumber < 1 || channelNumber > CHANNEL_COUNT) {
        qWarning() << "Invalid channel number:" << channelNumber;
        return;
    }

    QMutexLocker locker(&m_fileMutex);
    QFile* file = m_files[channelNumber - 1].get();

    if (!file || !file->isOpen()) {
        qWarning() << "File for channel" << channelNumber << "is not available";
        return;
    }

    QString str = QString::number(iQuadrature, 'f', 6) + ";" +
                  QString::number(qQuadrature, 'f', 6) + "\n";

    write(str, file);
}

void CsvParser::appendChannelDataBatch(int channelNumber, const QVector<QPair<double, double>> &data)
{
    if (channelNumber < 1 || channelNumber > CHANNEL_COUNT) {
        qWarning() << "Invalid channel number:" << channelNumber;
        return;
    }

    QMutexLocker locker(&m_fileMutex);
    QFile* file = m_files[channelNumber - 1].get();

    if (!file || !file->isOpen()) {
        qWarning() << "File for channel" << channelNumber << "is not available";
        return;
    }

    QString buffer;
    buffer.reserve(data.size() * 32);

    for (const auto &point : data) {
        buffer += QString::number(point.first, 'f', 6) + ";" +
                  QString::number(point.second, 'f', 6) + "\n";
    }

    write(buffer, file);
}

void CsvParser::write(const QString &str, QFile* file)
{
    static constexpr qint64 MAX_FILE_SIZE = 55'500'000;
    static constexpr int LINES_TO_REMOVE = 500'000;
    static constexpr int FLUSH_THRESHOLD = 8'192;

    if (file->size() > MAX_FILE_SIZE) {
        removeFirstNLines(file, LINES_TO_REMOVE);
    }

    QTextStream stream(file);
    stream << str;

    if (stream.status() != QTextStream::Ok) {
        qWarning() << "Failed to write data to file" << file->fileName();
        return;
    }

    if (file->pos() > FLUSH_THRESHOLD) {
        stream.flush();
    }
}

void CsvParser::removeFirstNLines(QFile *file, int n)
{
    if (!file || n <= 0 || !file->isOpen()) return;

    QTemporaryFile tempFile;
    if (!tempFile.open()) {
        qWarning() << "Failed to create temporary file";
        return;
    }

    file->seek(0);
    QTextStream in(file);
    QTextStream out(&tempFile);

    int linesSkipped = 0;
    while (linesSkipped < n && !in.atEnd()) {
        in.readLine();
        linesSkipped++;
    }

    out << in.readAll();

    if (!file->resize(0)) {
        qWarning() << "Failed to truncate file";
        return;
    }

    file->seek(0);
    tempFile.seek(0);
    file->write(tempFile.readAll());
}

bool CsvParser::reopenFile(QFile* file)
{
    if (!file) return false;

    QString filename = file->fileName();
    file->close();

    if (!file->open(QIODevice::Append | QIODevice::Text | QIODevice::ReadWrite)) {
        qWarning() << "Failed to reopen file:" << filename;
        return false;
    }

    return true;
}
