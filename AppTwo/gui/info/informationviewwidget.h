#pragma once

#include "server/countmessage.h"

#include <QWidget>

namespace View {

class ComplexCounts;
class InformationList;

class InformationViewWidget : public QWidget
{
public:
    explicit InformationViewWidget(QWidget *parent = nullptr);

    void addChannelData(ChannelData &data);
private:
    InformationList *m_infoList;
    ComplexCounts *m_complexCounts;
};

}
