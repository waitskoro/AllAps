#pragma once

#include <QObject>

#include "plotter.h"
#include "graphtracer.h"

class MarkersDataModel : public QAbstractListModel {

    Q_OBJECT

public:
    explicit MarkersDataModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

public slots:
    void populate(QVector<DataMarker> values);

private:
    QVector<DataMarker> m_values;
};

//=============================================================================

class MarkersPlotter : public Plotter
{
    Q_OBJECT
public:
    explicit MarkersPlotter(QCustomPlot* plotter, QObject *parent = nullptr);

public slots:
    void setUi(Ui::MainWindow *ui);
    void setMarkersUsage(bool usage)
    {
        m_tracer->setUsage(usage);
    }

private:
    Ui::MainWindow *m_ui;
    GraphTracer *m_tracer;
    MarkersDataModel *m_model;
};
