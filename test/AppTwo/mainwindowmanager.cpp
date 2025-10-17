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
        if (m_ui->lineEditPort->isEnabled()) {
            int port = m_ui->lineEditPort->text().toInt();
            emit createServer(port);
        } else {
            emit serverStopped();
            m_ui->lineEditPort->setEnabled(true);
            m_ui->pushButtonCreate->setText("Создать сервер");
        }
    });

    connect(m_ui->action16_bit, &QAction::triggered,
            [this](){
        auto checked = m_ui->action16_bit->isChecked();
        emit checkableChanged(checked);
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

void MainWindowManager::onCountMessageRecieved(const Report &msg)
{
    auto checked = m_ui->action16_bit->isChecked();
    m_infoManager->addInfo(checked, msg);

    m_graphManager->onSamplesReaded(msg.channel, convertToComplex(msg.info_16));

    double azimut = msg.az[0] / 10.0;
    double angle = msg.az[1] / 10.0;
    m_graphManager->setAzimutAngle(msg.channel, azimut, angle);
}

ComplexVector MainWindowManager::convertToComplex(const QVector<std::array<qint16, 2>> &data)
{
    ComplexVector result;

    for (const auto& item : data)
        result.append(Complex(item[0], item[1]));

    return result;
}
