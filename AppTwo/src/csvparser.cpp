#include "csvparser.h"

#include <QFile>
#include <QDebug>
#include <QStandardItem>
#include <QTextStream>

using namespace Reports;

CsvParser::CsvParser(QObject *parent)
    : QObject{parent}
    , m_file("../reports.csv")
{
    if (!m_file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "File not exists";
    } else {
        qDebug() << "File is exist";
    }
}

QVector<ChannelData> CsvParser::parseChannelData()
{
    QVector<ChannelData> channels;

    if (!m_file.isOpen()) {
        qWarning() << "File is not open for reading";
        return channels;
    }

    QTextStream in(&m_file);
    bool isFirstLine = true;

    while (!in.atEnd()) {
        QString line = in.readLine();

        if (isFirstLine) {
            isFirstLine = false;
            if (line.contains("channelNumber") || line.contains("синфазная составляющая") || line.contains("квадратурная составляющая")) {
                continue;
            }
        }

        QStringList fields = line.split(';');
        if (fields.size() >= 3) {
            ChannelData data;
            bool ok;

            data.channelNumber = fields[0].trimmed().toInt(&ok);
            if (!ok) continue;

            data.iQuadrature = fields[1].trimmed().toDouble(&ok);
            if (!ok) continue;

            data.qQuadrature = fields[2].trimmed().toDouble(&ok);
            if (!ok) continue;

            channels.append(data);
        }
    }

    m_file.close();
    return channels;
}

bool CsvParser::appendChannelData(int channelNumber, double iQuadrature, double qQuadrature)
{
    if (m_file.isOpen()) {
        m_file.close();
    }

    if (!m_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qWarning() << "Failed to open file for appending";
        return false;
    }

    QTextStream out(&m_file);

    if (m_file.size() == 0) {
        out << "channelNumber;синфазная составляющая (I-квадратура);квадратурная составляющая (Q-квадратура)\n";
    }

    out << channelNumber << ";"
        << QString::number(iQuadrature, 'f', 6) << ";"
        << QString::number(qQuadrature, 'f', 6) << "\n";

    m_file.close();
    return true;
}
