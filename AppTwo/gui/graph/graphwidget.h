#pragma once

#include <QLabel>
#include <QWidget>
#include <QComboBox>
#include <QChartView>
#include <QHBoxLayout>

#include "common/messages.h"

namespace View {

namespace Common {
class CustomComboBox;
}

namespace Graphic {

class Plotter;

class GraphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GraphWidget(QWidget *parent = nullptr);

    void addItemThreadSafe(const Report &msg);

signals:
    void itemAdded(const Report &msg);

private:
    QVBoxLayout *m_layout;
    Plotter *m_powerGraph;
    Plotter *m_signalGraph;
    Plotter *m_spectrumGraph;

    QLabel *m_mainText;

    Common::CustomComboBox *m_channelsBox;

    Q_INVOKABLE void addItem(const Report &msg);
};

}

}
