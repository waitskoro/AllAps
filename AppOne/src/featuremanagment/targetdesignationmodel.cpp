#include "targetdesignationmodel.h"

using namespace View;

TargetDesignationModel::TargetDesignationModel(QObject *parent)
    : QAbstractListModel(parent)
{}

void TargetDesignationModel::append(const TargetDesignation& newCoordinates)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_values.push_back(newCoordinates);
    endInsertRows();
}

void TargetDesignationModel::clear()
{
    if (m_values.empty())
        return;

    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    m_values.clear();
    endRemoveRows();
}

void TargetDesignationModel::removeRow(int row)
{
    if (row < 0 || static_cast<int>(row) >= m_values.size())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_values.erase(m_values.begin() + row);
    endRemoveRows();
}

void TargetDesignationModel::append(double azimuth, double elevation)
{
    append({azimuth, elevation});
}

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
        static_cast<int>(index.row()) >= m_values.size())
    {
        return QVariant();
    }

    const auto& coords = m_values.at(index.row());

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
            case 0: return coords.azimut;
            case 1: return coords.elev;
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

const QVector<TargetDesignation> &TargetDesignationModel::coordinates() const
{
    return m_values;
}

int TargetDesignationModel::countCoordinates() const
{
    return m_values.size();
}
