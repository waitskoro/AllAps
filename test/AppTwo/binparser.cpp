#include "binparser.h"

#include <QDebug>
#include <cstdio>

BinParser::BinParser(QObject *parent)
    : QObject(parent)
    , m_thread(std::make_unique<QThread>())
{
    // Инициализируем массив файлов nullptr
    m_files.fill(nullptr);

    moveToThread(m_thread.get());
    m_thread->start();

    for (int i = 0; i < CHANNEL_COUNT; ++i) {
        QString filename = QString("../reports_%1.bin").arg(i+1);

        // Открываем файл в бинарном режиме для чтения и записи
        FILE* file = fopen(filename.toLocal8Bit().constData(), "ab+");
        if (!file) {
            qWarning() << "Failed to open file:" << filename;
            continue;
        }

        m_files[i] = file;
    }
}

BinParser::~BinParser()
{
    m_thread->requestInterruption();
    m_thread->quit();

    if (!m_thread->wait(1000)) {
        qWarning() << "Forcing thread termination";
        m_thread->terminate();
    }

    QMutexLocker locker(&m_fileMutex);
    for (auto& file : m_files) {
        if (file) {
            fclose(file);
            file = nullptr;
        }
    }
}

void BinParser::appendChannelData(int channelNumber, int iQuadrature, int qQuadrature)
{
    if (channelNumber < 1 || channelNumber > CHANNEL_COUNT) {
        qWarning() << "Invalid channel number:" << channelNumber;
        return;
    }

    QMutexLocker locker(&m_fileMutex);
    FILE* file = m_files[channelNumber - 1];

    if (!file) {
        qWarning() << "File for channel" << channelNumber << "is not available";
        return;
    }

    DataPoint point = {iQuadrature, qQuadrature};
    write(&point, 1, file);
}

void BinParser::appendChannelDataBatch(int channelNumber, const QVector<QPair<int, int>> &data)
{
    if (channelNumber < 1 || channelNumber > CHANNEL_COUNT) {
        qWarning() << "Invalid channel number:" << channelNumber;
        return;
    }

    QMutexLocker locker(&m_fileMutex);
    FILE* file = m_files[channelNumber - 1];

    if (!file) {
        qWarning() << "File for channel" << channelNumber << "is not available";
        return;
    }

    // Преобразуем данные в бинарный формат
    QVector<DataPoint> binaryData;
    binaryData.reserve(data.size());

    for (const auto &point : data) {
        binaryData.append({point.first, point.second});
    }

    write(binaryData.constData(), binaryData.size(), file);
}

void BinParser::write(const DataPoint* data, size_t count, FILE* file)
{
//    static constexpr long MAX_FILE_SIZE = 55'000'000;
//    static constexpr int POINTS_TO_REMOVE = 500'000;
    static constexpr size_t FLUSH_THRESHOLD = 8'192 / sizeof(DataPoint);

//    // Получаем текущий размер файла
//    long currentPos = ftell(file);
//    if (currentPos == -1) {
//        qWarning() << "Failed to get file position";
//        return;
//    }

//    // Проверяем размер файла
//    fseek(file, 0, SEEK_END);
//    long fileSize = ftell(file);
//    fseek(file, currentPos, SEEK_SET); // Возвращаем позицию

//    if (fileSize > MAX_FILE_SIZE) {
//        removeFirstNPoints(file, POINTS_TO_REMOVE);
//    }

    // Записываем данные
    size_t written = fwrite(data, sizeof(DataPoint), count, file);

    if (written != count) {
        qWarning() << "Failed to write data to file, written:" << written << "of" << count;
        return;
    }

    // Периодически сбрасываем буфер
    static size_t writeCounter = 0;
    writeCounter += count;

    if (writeCounter > FLUSH_THRESHOLD) {
        fflush(file);
        writeCounter = 0;
    }
}

void BinParser::removeFirstNPoints(FILE* file, int n)
{
    if (!file || n <= 0) return;

    // Получаем размер файла
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);

    if (fileSize <= 0) return;

    // Вычисляем сколько байт нужно удалить
    long bytesToRemove = n * sizeof(DataPoint);
    if (bytesToRemove >= fileSize) {
        // Если нужно удалить больше чем есть - очищаем файл полностью
        freopen(nullptr, "wb", file);
        return;
    }

    // Читаем оставшиеся данные
    fseek(file, bytesToRemove, SEEK_SET);
    long remainingSize = fileSize - bytesToRemove;

    QByteArray buffer(remainingSize, 0);
    size_t read = fread(buffer.data(), 1, remainingSize, file);

    if (read != static_cast<size_t>(remainingSize)) {
        qWarning() << "Failed to read data during truncation";
        return;
    }

    // Перезаписываем файл
    freopen(nullptr, "wb", file);
    size_t written = fwrite(buffer.constData(), 1, remainingSize, file);

    if (written != static_cast<size_t>(remainingSize)) {
        qWarning() << "Failed to write data during truncation";
    }
}

bool BinParser::reopenFile(FILE*& file, const QString& filename)
{
    if (file) {
        fclose(file);
        file = nullptr;
    }

    file = fopen(filename.toLocal8Bit().constData(), "ab+");
    if (!file) {
        qWarning() << "Failed to reopen file:" << filename;
        return false;
    }

    return true;
}
