#include "graphwidget.h"

#include "plotter.h"
#include "gui/common/customcombobox.h"

#include <QValueAxis>
#include <QLineSeries>

using namespace View::Graphic;
using namespace View::Common;

GraphWidget::GraphWidget(QWidget *parent)
    : QWidget{parent}
    , m_layout(new QVBoxLayout(this))
    , m_iGraph(new Plotter(this))
    , m_qGraph(new Plotter(this))
    , m_iqGraph(new Plotter(this))
    , m_channelsBox(new CustomComboBox(this))
{
    m_layout->addWidget(m_channelsBox);

    QLabel *label_1 = new QLabel("I-квадратура", this);
    label_1->setFixedHeight(20);

    QLabel *label_2 = new QLabel("Q-квадратура", this);
    label_2->setFixedHeight(20);

    QLabel *label_3 = new QLabel("Спектр I/Q", this);
    label_3->setFixedHeight(20);

    m_layout->addWidget(label_1);
    m_layout->addWidget(m_iGraph);

    m_layout->addWidget(label_2);
    m_layout->addWidget(m_qGraph);

    m_layout->addWidget(label_3);
    m_layout->addWidget(m_iqGraph);

    m_iGraph->graph(0)->setPen(QPen(Qt::blue));
    m_qGraph->graph(0)->setPen(QPen(Qt::red));
    m_iqGraph->graph(0)->setPen(QPen(Qt::green));

    m_layout->setAlignment(m_channelsBox, Qt::AlignHCenter);

    connect(m_channelsBox,
            &CustomComboBox::checkedChannelsChanged,
            [this](QList<qint8> &list){
                m_iGraph->addChannels(list);
                m_qGraph->addChannels(list);
                m_iqGraph->addChannels(list);

                qDebug() << list.count();
            });
}

void GraphWidget::addItem(const Report &msg)
{
    m_iGraph->addItem(msg, Plotter::Type::I);
    m_qGraph->addItem(msg, Plotter::Type::Q);
    m_iqGraph->addItem(msg, Plotter::Type::Spectrum);
}
