#include "graphwidget.h"

#include <QBoxLayout>

#include "plotter.h"
#include "dftplotter.h"
#include "timeplotter.h"
#include "graphtracer.h"
#include "powerplotter.h"

namespace {

QLabel* createFixedHeightLabel(const QString& text, QWidget* parent, int height = 20)
{
    QLabel* label = new QLabel(text, parent);
    label->setFixedHeight(height);
    return label;
}

}

using namespace View::Graphic;

GraphWidget::GraphWidget(QWidget *parent)
    : QWidget(parent)
    , m_layout(new QVBoxLayout(this))
    , m_hLayout(new QHBoxLayout())
    , m_channelData(new QComboBox(this))
    , m_dftPlotter(new Graph::DftPlotter())
    , m_timePlotterI(new Graph::TimePlotter())
    , m_timePlotterQ(new Graph::TimePlotter())
    , m_powerPlotter(new Graph::PowerPlotter())
{
    connect(m_dftPlotter->tracer, &GraphTracer::positionUpdate,
            this, [this] {
        m_dftGraphAmpTracerMin = 1e10;
        m_dftGraphAmpTracerAvg = 0;
        m_dftGraphAmpTracerMax = 0;
        m_dftGraphAmpTracerCrt = 0;
    });

    connect(m_dftPlotter->tracer, &GraphTracer::valueUpdate,
            this, [this] {
        m_dftGraphAmpTracerCrt = m_dftPlotter->tracer->getValue();

        if (m_dftGraphAmpTracerCrt > m_dftGraphAmpTracerMax)
            m_dftGraphAmpTracerMax = m_dftGraphAmpTracerCrt;
        if (m_dftGraphAmpTracerCrt < m_dftGraphAmpTracerMin)
            m_dftGraphAmpTracerMin = m_dftGraphAmpTracerCrt;
        m_dftGraphAmpTracerAvg = (m_dftGraphAmpTracerMax + m_dftGraphAmpTracerMin)/2;
    });

    for (int i = 0; i < 12; i++)
        m_channelData->addItem(QString::number(i + 1), QVariant(i));

    init();
}

GraphWidget::~GraphWidget()
{}

void GraphWidget::init()
{
    m_layout->addWidget(m_channelData);
    m_layout->addWidget(createFixedHeightLabel("Отображение Q и I графиков", this));

    m_hLayout->addWidget(m_timePlotterI->plotter);
    m_hLayout->addWidget(m_timePlotterQ->plotter);
    m_layout->addLayout(m_hLayout);

    m_layout->addWidget(createFixedHeightLabel("Мощность сигнала Q/I", this));
    m_layout->addWidget(m_powerPlotter->plotter);

    m_layout->addWidget(createFixedHeightLabel("Спектр сигнала Q/I", this));

    m_layout->setAlignment(m_channelData, Qt::AlignHCenter);
    m_layout->addWidget(m_dftPlotter->plotter);

    connect(m_channelData, &QComboBox::currentIndexChanged, [this]() {
        m_channel = m_channelData->currentIndex() + 1;
    });
}

void GraphWidget::setChannel(int channel)
{
    m_channel = channel;
}

bool GraphWidget::channelMatch(int channel)
{
    return m_channel == channel;
}

void GraphWidget::onSamplesReaded(QVector<std::complex<double>> dataComplex)
{
    if (dataComplex.size() == 0)
        return;

    recreateWindow(dataComplex.size());

    dsp.input(dataComplex,1./110.e3);

    m_timePlotterI->setData(dsp.timeVector(), dsp.i());
    m_timePlotterI->plotter->yAxis->setLabel("I");

    m_timePlotterQ->setData(dsp.timeVector(), dsp.q());
    m_timePlotterQ->plotter->yAxis->setLabel("Q");

    m_powerPlotter->addData(dsp.powerFreqDb());
    m_dftPlotter->setData(dsp.freqVector(), dsp.ampDistDb());
}

void GraphWidget::recreateWindow(int size)
{
    if (recreateWindowNext == false) return;
    recreateWindowNext = false;

    dsp.window().resize(size);

    return;
}
