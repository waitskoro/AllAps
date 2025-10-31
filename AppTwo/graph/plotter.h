#pragma once

#include <QObject>

#include "../qcustomplot/qcustomplot.h"

class QCPGraph;
class QCustomPlot;

namespace Ui {
class MainWindow;
}

class Plotter : public QObject
{
    Q_OBJECT
public:
    explicit Plotter(QCustomPlot* plotter, QObject *parent = nullptr);

public slots:
    bool getRescale()
    {
        return m_rescale;
    }

    QCPGraph *getGraph()
    {
        return m_graph;
    }

    QCustomPlot *getPlotter()
    {
        return m_plotter;
    }

    void setRescale()
    {
        m_rescale = !m_rescale;
    }

    void setPlotter(QCustomPlot *plotter)
    {
        m_plotter = plotter;
    }

    void setYLabel(const QString &label)
    {
        m_plotter->yAxis->setLabel(label);
    }

    void setXLabel(const QString &label)
    {
        m_plotter->xAxis->setLabel(label);
    }

    void setXTicker(QSharedPointer<QCPAxisTickerDateTime> ticker)
    {
        m_plotter->xAxis->setTicker(ticker);
    }

    void clearGraph();
    void setData(const QVector<double> &y, const QVector<double> &x);

signals:
    void dataUpdated();

private:
    bool m_rescale = true;

    QCustomPlot *m_plotter;
    QCPGraph *m_graph;

};

