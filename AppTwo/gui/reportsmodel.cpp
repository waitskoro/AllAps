#include "reportsmodel.h"

using namespace View;

ReportsModel::ReportsModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int ReportsModel::rowCount(const QModelIndex &) const
{
    return m_values.size();
}

int ReportsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant ReportsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 ||
        static_cast<size_t>(index.row()) >= m_values.size())
    {
        return QVariant();
    }

    const auto& coords = m_values.at(index.row());

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
            case 0: return coords[0];
            case 1: return coords[1];
            default: return QVariant();
        }
    }

    return QVariant();
}

QVariant ReportsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
            case 0: return QString("Азимут");
            case 1: return QString("Угол места");
            default: return QString("");
        }
    }
    return QVariant();
}

void ReportsModel::clear()
{
    m_values.clear();
}

void ReportsModel::append(short azimuth, short elevation)
{
    append({azimuth, elevation});
}

void ReportsModel::append(const std::array<short, 2>& newCoordinates)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_values.push_back(newCoordinates);
    endInsertRows();
}

const std::vector<std::array<short, 2>>& ReportsModel::coordinates() const
{
    return m_values;
}
