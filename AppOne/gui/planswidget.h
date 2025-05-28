#pragma once

#include <QWidget>

#include "src/messagesinfo.h"
#include "gui/plansmanagment/planslist.h"

namespace View {

class PlansWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlansWidget(QWidget *parent = nullptr);

    void addMessage(const ReceivingMessage &);
private:
    void paintEvent(QPaintEvent *event);

    QScopedPointer<PlansList> m_receivingChecklist;
};

}
