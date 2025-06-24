#pragma once

#include "common/messages.h"

#include <QWidget>

namespace View {

class InformationList;

class InfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InfoWidget(QWidget *parent = nullptr);

    void addItem(const Report &msg);

private:
    void paintEvent(QPaintEvent *e) override;

    InformationList *m_infoList;
};

}
