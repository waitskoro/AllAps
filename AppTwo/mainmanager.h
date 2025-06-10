#pragma once

#include <QObject>

namespace View {
class MainWindow;
}

namespace Tcp {
class TcpManager;
}

class MainManager : public QObject
{
    Q_OBJECT

public:
    explicit MainManager(QObject *parent = nullptr);
    ~MainManager();

private:
    View::MainWindow *m_mainView;
    Tcp::TcpManager *m_tcpManager;
};
