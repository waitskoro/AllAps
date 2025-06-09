#include "csvparser.h"

#include <QFile>
#include <QDebug>
#include <QStandardItem>
#include <QTextStream>

using namespace Reports;

CsvParser::CsvParser(QObject *parent)
    : QObject{parent}
{}

bool CsvParser::appendChannelData(int channelNumber, double iQuadrature, double qQuadrature)
{
    m_file = fopen("../reports.csv", "a");
    if (!m_file) {
        qDebug() << "Failed to open file";
        return false;
    }

    QString str = QString::number(channelNumber) + ";" +
                  QString::number(iQuadrature, 'f', 6) + ";" +
                  QString::number(qQuadrature, 'f', 6) + "\n";

    QByteArray byteArray = str.toUtf8();
    const char *buffer = byteArray.constData();
    size_t bytesToWrite = byteArray.size();

    size_t written = fwrite(buffer, 1, bytesToWrite, m_file);
    if (written != bytesToWrite) {
        qDebug() << "Failed to write all data";
        fclose(m_file);
        return false;
    }

    fclose(m_file);
    return true;
}
