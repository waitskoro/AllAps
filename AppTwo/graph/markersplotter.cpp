#include "markersplotter.h"

#include <QModelIndex>
#include <QAbstractListModel>

MarkersDataModel::MarkersDataModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

int MarkersDataModel::rowCount(const QModelIndex &) const
{
    if (m_values.isEmpty())
        return 0;

    return m_values[0].data.count();
}

int MarkersDataModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if (m_values.isEmpty())
        return 0;

    return m_values.count();
}

QVariant MarkersDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() ||
        index.column() >= m_values.count() ||
        index.row() >= rowCount(index)) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        QString valueStr = m_values[index.column()].data[index.row()];

        bool ok;
        double value = valueStr.toDouble(&ok);

        if (ok) {
            return QString::number(value, 'f', 2);
        } else {
            return valueStr;
        }
    }

    return QVariant();
}

QVariant MarkersDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return section + 1;
    }

    return QVariant();
}

void MarkersDataModel::populate(QVector<DataMarker> values)
{
    beginResetModel();
    for (auto &data : values)
        std::reverse(data.data.begin(), data.data.end());
    m_values = values;
    endResetModel();
}

//==============================================================================

#include "ui_mainwindow.h"

MarkersPlotter::MarkersPlotter(QCustomPlot* plotter, QObject *parent)
    : Plotter(plotter, parent)
    , m_tracer(new GraphTracer(getGraph(), this))
    , m_model(new MarkersDataModel(this))
{
    connect(this, &Plotter::dataUpdated, [this](){
        m_tracer->onDataUpdate();
        auto data = m_tracer->getAllMarkersData();
        m_model->populate(data);
    });
}

void MarkersPlotter::setUi(Ui::MainWindow *ui)
{
    m_ui = ui;

    ui->tableView_2->setModel(m_model);
    ui->tableView_2->resizeColumnsToContents();
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(m_ui->pushButtonCleanMarkers,
            &QPushButton::clicked,
            m_tracer,
            &GraphTracer::reset);
}
