#include "csvparser.h"

#include <QFile>
#include <QDebug>
#include <QThread>
#include <QTextStream>
#include <QStandardItem>
#include <QTemporaryFile>

using namespace Reports;

CsvParser::CsvParser(QObject *parent)
    : QObject(parent)
    , m_thread(new QThread(this))
{
    for (int i = 0; i < CHANNEL_COUNT; ++i) {
        QString filename = QString("../reports_%1.csv").arg(i+1);
        m_files[i] = new QFile(filename);
        if (!m_files[i]->open(QIODevice::Append |
                              QIODevice::Text |
                              QIODevice::Truncate |
                              QIODevice::ReadWrite)) {
            qDebug() << "Failed to open file:" << filename;
        }
    }

    moveToThread(m_thread);
    m_thread->start();
}

CsvParser::~CsvParser()
{
    for (int i = 0; i < CHANNEL_COUNT; ++i) {
        if (m_files[i]) {
            m_files[i]->close();
            delete m_files[i];
        }
    }
}

void CsvParser::appendChannelData(int channelNumber,
                                  int iQuadrature,
                                  int qQuadrature)
{
    if (channelNumber < 1 || channelNumber > CHANNEL_COUNT) {
        qDebug() << "Invalid channel number:" << channelNumber;
        return;
    }

    QFile* file = m_files[channelNumber - 1];
    if (!file || !file->isOpen()) {
        qDebug() << "File for channel" << channelNumber << "is not open";
        return;
    }

    QString str = QString::number(iQuadrature, 'f', 6) + ";" +
                  QString::number(qQuadrature, 'f', 6) + "\n";

    write(str, file);
}

void CsvParser::appendChannelDataBatch(int channelNumber,
                                       const QVector<QPair<double, double>> &data)
{
    if (channelNumber < 1 || channelNumber > CHANNEL_COUNT) {
        qDebug() << "Invalid channel number:" << channelNumber;
        return;
    }

    QFile* file = m_files[channelNumber - 1];
    if (!file || !file->isOpen()) {
        qDebug() << "File for channel" << channelNumber << "is not open";
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

void CsvParser::write(QString str, QFile* file)
{
    if (file->size() > 55500000) {
        removeFirstNLines(file, 500000);
    }

    QTextStream stream(file);
    stream << str;

    if (stream.status() != QTextStream::Ok) {
        qDebug() << "Failed to write data";
    }
}

void CsvParser::removeFirstNLines(QFile *file, int n) {
    if (!file || n <= 0 || !file->isOpen()) return;

    file->close();
    if (!file->open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Failed to reopen file for truncation";
        return;
    }

    QTextStream in(file);
    QString content;
    int linesSkipped = 0;

    while (linesSkipped < n && !in.atEnd()) {
        in.readLine();
        linesSkipped++;
    }

    content = in.readAll();
    file->resize(0);

    QTextStream out(file);
    out << content;
    file->close();

    if (!file->open(QIODevice::Append | QIODevice::Text | QIODevice::ReadWrite)) {
        qDebug() << "Failed to reopen file in append mode";
    }
}
