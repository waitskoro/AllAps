#include "mainmanager.h"

#include "src/csvparser.h"
#include "src/tcpmanager.h"
#include "gui/mainwindow.h"

MainManager::MainManager(QObject *parent)
    : QObject(parent)
    , m_mainView(new View::MainWindow())
    , m_tcpManager(new Tcp::TcpManager(this))
    , m_csvParser(new Reports::CsvParser(this))
{
    m_mainView->show();

    connect(m_mainView,
            &View::MainWindow::createServer,
            m_tcpManager,
            &Tcp::TcpManager::onServerCreating);

    connect(m_tcpManager,
            &Tcp::TcpManager::serverCreated,
            m_mainView,
            &View::MainWindow::onServerCreated);

    connect(m_tcpManager,
            &Tcp::TcpManager::clientConnected,
            m_mainView,
            &View::MainWindow::onClientConnected);

    connect(m_tcpManager,
            &Tcp::TcpManager::countMessage,
            [this](Report result){
                m_mainView->onCountMessageRecieved(result);

                for (quint32 i = 0; i < result.count; i++) {
                    m_csvParser->appendChannelData(result.dataChannelNumber,
                                                   result.info[i][0],
                                                   result.info[i][1]);
                }

            });
}

MainManager::~MainManager()
{}
