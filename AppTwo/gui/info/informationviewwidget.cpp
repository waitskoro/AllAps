#include "informationviewwidget.h"

#include "complexcounts.h"
#include "informationlist.h"

using namespace View;

InformationViewWidget::InformationViewWidget(QWidget *parent)
    : QWidget(parent)
    , m_infoList(new InformationList(this))
    , m_complexCounts(new ComplexCounts(this))
{
    m_infoList->move(20, 40);
    m_complexCounts->move(400, 40);
}

void InformationViewWidget::addChannelData(ChannelData &data)
{
    m_infoList->addInfo(data);
}
