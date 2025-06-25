#pragma once

#include <QWidget>
#include <QComboBox>
#include <QChartView>
#include <QHBoxLayout>

namespace View::Graphic {

class Plotter;

class GraphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GraphWidget(QWidget *parent = nullptr);

signals:

private:
    QVBoxLayout *m_layout;
    Plotter *m_iGraph;
    Plotter *m_qGraph;
    Plotter *m_iqGraph;

    QComboBox *m_channelsBox;
};

}
