#pragma once

#include <QWidget>

namespace View {

class ComplexCounts;
class InformationList;

class InformationViewWidget : public QWidget
{
public:
    explicit InformationViewWidget(QWidget *parent = nullptr);

private:
    InformationList *m_infoList;
    ComplexCounts *m_complexCounts;
};

}
