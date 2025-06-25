#include "graphwidget.h"

#include <QValueAxis>
#include <QLineSeries>

#include "plotter.h"

using namespace View::Graphic;

GraphWidget::GraphWidget(QWidget *parent)
    : QWidget{parent}
    , m_layout(new QVBoxLayout(this))
    , m_iGraph(new Plotter())
    , m_qGraph(new Plotter())
    , m_iqGraph(new Plotter())
    , m_channelsBox(new QComboBox(this))
{
    m_layout->addWidget(m_channelsBox);

    m_layout->addWidget(m_iGraph);
    m_layout->addWidget(m_qGraph);
    m_layout->addWidget(m_iqGraph);
}
