#pragma once

#include "common/messages.h"

#include <QWidget>

namespace View {

class TargetTable;
class InformationList;

class InfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InfoWidget(QWidget *parent = nullptr);

    void addItem(Report msg);

private:
    void paintEvent(QPaintEvent *e) override;

    TargetTable *m_targetList;
    InformationList *m_infoList;
};

}
