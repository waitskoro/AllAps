#pragma once

#include <QWidget>

namespace View {

class GraphsViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GraphsViewWidget(QWidget *parent = nullptr);
};

}
