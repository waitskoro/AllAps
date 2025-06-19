#pragma once

#include <QWidget>

namespace View {

class CamRadiatorsList;

class StateCamRadiators : public QWidget
{
public:
    explicit StateCamRadiators(QWidget *parent = nullptr);

    void setInfo(qint32 count, QVariant &var);

private:
    void paintEvent(QPaintEvent *event);

    CamRadiatorsList *m_list;
};

}
