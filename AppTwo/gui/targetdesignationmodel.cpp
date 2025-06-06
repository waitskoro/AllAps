#include "targetdesignationmodel.h"

using namespace View;

TargetDesignationModel::TargetDesignationModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int TargetDesignationModel::rowCount(const QModelIndex &) const
{
    return m_values.size();
}

int TargetDesignationModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant TargetDesignationModel::data(const QModelIndex &index, int role) const
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

QVariant TargetDesignationModel::headerData(int section, Qt::Orientation orientation, int role) const
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

void TargetDesignationModel::append(short azimuth, short elevation)
{
    append({azimuth, elevation});
}

void TargetDesignationModel::append(const std::array<short, 2>& newCoordinates)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_values.push_back(newCoordinates);
    endInsertRows();
}

const std::vector<std::array<short, 2>>& TargetDesignationModel::coordinates() const
{
    return m_values;
}
