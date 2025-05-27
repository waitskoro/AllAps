#pragma once

#include <QWidget>
#include <QPushButton>

namespace View {

class ConnectionHeaderBlock : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionHeaderBlock(QWidget *parent = nullptr);

signals:
    void disconnect();
};

}
