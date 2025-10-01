#include "mainwindowmanager.h"

#include "ui_mainwindow.h"
#include "graph/graphmanager.h"
#include "info/infomanager.h"

MainWindowManager::MainWindowManager(QObject *parent)
    : QObject(parent)
    , m_infoManager(new InfoManager(this))
    , m_graphManager(new GraphManager(this))
{}

void MainWindowManager::setUi(Ui::MainWindow *ui)
{
    m_ui = ui;
    m_graphManager->setUi(ui);
    m_infoManager->setListView(ui->listView);

    connect(m_ui->pushButtonCreate, &QPushButton::clicked, [this](){
        int port = m_ui->lineEditPort->text().toInt();
        emit createServer(port);
    });
}

void MainWindowManager::setWidget(QWidget *widget)
{
    m_widget = widget;
}

void MainWindowManager::onServerCreated()
{
    m_ui->lineEditPort->setEnabled(false);
    m_ui->pushButtonCreate->setText("Отключить сервер");
}

void MainWindowManager::onClientConnected()
{

}

void MainWindowManager::onCountMessageRecieved(const Report &msg)
{
    if (msg.channel == m_ui->channel->currentIndex() + 1) {
        m_graphManager->onSamplesReaded(convertToComplex(msg.info));
    }

    m_infoManager->addInfo(msg);
}

ComplexVector MainWindowManager::convertToComplex(const QVector<std::array<qint8, 2>> &data)
{
    ComplexVector result;

    for (const auto& item : data)
        result.append(Complex(item[0], item[1]));

    return result;
}
