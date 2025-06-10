#include "csvparser.h"

#include <QFile>
#include <QDebug>
#include <QThread>
#include <QStandardItem>
#include <QTextStream>

using namespace Reports;

CsvParser::CsvParser(QObject *parent)
    : QObject(parent)
    , m_thread(new QThread())
{
    for (int i = 0; i < CHANNEL_COUNT; ++i) {
        QString filename = QString("../reports_%1.csv").arg(i+1);
        m_files[i] = fopen(filename.toLocal8Bit().constData(), "a");
        if (!m_files[i]) {
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
            fclose(m_files[i]);
        }
    }
}

bool CsvParser::appendChannelData(int channelNumber,
                                  double iQuadrature,
                                  double qQuadrature)
{
    if (channelNumber < 1 || channelNumber > CHANNEL_COUNT) {
        qDebug() << "Invalid channel number:" << channelNumber;
        return false;
    }

    FILE* file = m_files[channelNumber - 1];
    if (!file) {
        qDebug() << "File for channel" << channelNumber << "is not open";
        return false;
    }

    QString str = QString::number(iQuadrature, 'f', 6) + ";" +
                  QString::number(qQuadrature, 'f', 6) + "\n";

    return write(str, file);
}

bool CsvParser::write(QString str, FILE *__restrict file)
{
    QByteArray byteArray = str.toUtf8();
    const char *buffer = byteArray.constData();
    size_t bytesToWrite = byteArray.size();

    size_t written = fwrite(buffer, 1, bytesToWrite, file);
    if (written != bytesToWrite) {
        qDebug() << "Failed to write all data";
        return false;
    }

    return true;
}
