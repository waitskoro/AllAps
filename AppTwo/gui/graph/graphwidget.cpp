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
    , m_powerGraph(new Plotter(this))
    , m_signalGraph(new Plotter(this))
    , m_spectrumGraph(new Plotter(this))
    , m_channelsBox(new CustomComboBox(this))
{
    m_layout->addWidget(m_channelsBox);

    m_mainText = new QLabel("", this);
    m_mainText->setFixedHeight(20);
    m_layout->addWidget(m_mainText);

    QLabel *label_1 = new QLabel("Мощность", this);
    label_1->setFixedHeight(20);

    QLabel *label_2 = new QLabel("Сигнал", this);
    label_2->setFixedHeight(20);

    QLabel *label_3 = new QLabel("Спектр", this);
    label_3->setFixedHeight(20);

    m_layout->addWidget(label_1);
    m_layout->addWidget(m_powerGraph);

    m_layout->addWidget(label_2);
    m_layout->addWidget(m_signalGraph);

    m_layout->addWidget(label_3);
    m_layout->addWidget(m_spectrumGraph);

    m_layout->setAlignment(m_mainText, Qt::AlignHCenter);
    m_layout->setAlignment(m_channelsBox, Qt::AlignHCenter);

    connect(m_channelsBox,
            &CustomComboBox::checkedChannelsChanged,
            [this](QVector<qint8> &list){
                m_powerGraph->addChannels(list);
                m_signalGraph->addChannels(list);
                m_spectrumGraph->addChannels(list);
            });
}

void GraphWidget::addItem(const Report &msg)
{
    m_powerGraph->addItem(msg, Plotter::Power);
    m_signalGraph->addItem(msg, Plotter::Signal);
    m_spectrumGraph->addItem(msg, Plotter::Spectrum);

    auto text = QString("Max мощность: %1     Max сигнал: %2    Max спектр: %3");
    m_mainText->setText(text.arg(m_powerGraph->maxPower())
                            .arg(m_signalGraph->maxSignal())
                            .arg(m_spectrumGraph->maxSpectrum()));
}
