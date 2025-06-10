#pragma once

#include <QLabel>
#include <QWidget>

#include "src/messagesinfo.h"
#include "gui/plansmanagment/planslist.h"

namespace View {

class PlansWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlansWidget(QWidget *parent = nullptr);

    void clear();
    void addMessage(const ReceivingMessage &);
private:
    void paintEvent(QPaintEvent *event);

    QScopedPointer<PlansList> m_receivingChecklist;

    QLabel *m_headerLabel;
};

}
