#pragma once

#include "src/common.h"

#include <QVector>
#include <QAbstractListModel>

namespace View {

class TargetDesignationModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TargetDesignationModel(QObject *parent = nullptr);

    void clear();

    void removeRow(int row);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void append(double azimuth, double elevation);
    void append(const TargetDesignation& newCoordinates);

    int countCoordinates() const;
    const QVector<TargetDesignation> &coordinates() const;

private:
    QVector<TargetDesignation> m_values;
};

}
