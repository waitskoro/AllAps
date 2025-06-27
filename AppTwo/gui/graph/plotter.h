#pragma once

#include <QMap>
#include <QWidget>

#include "qcustomplot.h"
#include "common/messages.h"

class QCPGraph;

namespace View::Graphic {


class Plotter : public QCustomPlot
{
    Q_OBJECT
public:
    enum Type {
        Q,
        I,
        Spectrum
    };

    explicit Plotter(QWidget *parent = nullptr);

    void addChannels(const QList<qint8>);
    void addItem(const Report &msg, Type);
    void setupGraph(const QString &name, const QColor &color);
signals:

private:
    void convertToComplex();

    QList<qint8> *m_channels;
};

}
