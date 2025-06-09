#pragma once

#include <QObject>

namespace View {
class MainWindow;
}

namespace Tcp {
class TcpManager;
}

namespace Reports {
class CsvParser;
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
    Reports::CsvParser *m_csvParser;
};
