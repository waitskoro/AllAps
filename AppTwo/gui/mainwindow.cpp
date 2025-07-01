#include "mainwindow.h"

#include <QTimer>
#include <QPainter>
#include <QDateTime>

#include "infowidget.h"
#include "graph/graphwidget.h"
#include "serverconnectingwidget.h"

using namespace View;
using namespace View::Graphic;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_label(new QLabel(this))
    , m_tabWidget(new QTabWidget(this))
    , m_infoViewer(new InfoWidget(this))
    , m_graphViewer(new GraphWidget(this))
    , m_serverConnecting(new ServerConnectingWidget(this))
{
    init();

    m_tabWidget->hide();

    connect(m_serverConnecting,
            &ServerConnectingWidget::createServer,
            this,
            &MainWindow::createServer);

    m_reportTimer = new QTimer(this);
    connect(m_reportTimer, &QTimer::timeout, this, &MainWindow::generateReport);
}

MainWindow::~MainWindow()
{}

void MainWindow::onServerCreated()
{
    m_label->hide();
    m_serverConnecting->hide();

    m_tabWidget->show();

    m_reportTimer->start(1000);

    generateReport();
}

void MainWindow::onClientConnected()
{
    qDebug() << "Клиент подключился";
}

void MainWindow::onCountMessageRecieved(const Report &msg)
{
    m_infoViewer->addItem(msg);
    m_graphViewer->addItem(msg);
}

void MainWindow::generateReport()
{
    Report r;

    m_currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;

    r.time = m_currentTime;

    int end = -127;
    int start = 127;
    qint8 x = rand() % (end - start + 1) + start;
    qint8 y = rand() % (end - start + 1) + start;

    r.info.push_back(std::array<qint8, 2> {x, y});

    x = rand() % (end - start + 1) + start;
    y = rand() % (end - start + 1) + start;

    r.info.push_back(std::array<qint8, 2> {x, y});

    x = rand() % (end - start + 1) + start;
    y = rand() % (end - start + 1) + start;

    r.info.push_back(std::array<qint8, 2> {x, y});

    r.dataChannelNumber = 1;
    r.acState = 0;
    r.kaNumber = 1234;
    r.az = {0, 0};
    r.count = r.info.size();

    m_graphViewer->addItem(r);

    r.info.clear();
    x = rand() % (end - start + 1) + start;
    y = rand() % (end - start + 1) + start;
    r.info.push_back(std::array<qint8, 2> {x, y});
    r.dataChannelNumber = 2;
    m_graphViewer->addItem(r);
}

void MainWindow::init()
{
    setFixedSize(700, 800);

    m_serverConnecting->show();
    m_serverConnecting->move(120, 10);

    m_label->setFixedSize(rect().size());
    m_label->setStyleSheet("color: white;"
                           "font-weight: 900;"
                           "font-size: 36px;");
    m_label->setText("Подключитесь к серверу для \nдоступа к функционалу");
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setWordWrap(true);

    m_tabWidget->move(15, 50);
    m_tabWidget->setFixedSize(670, 700);
    m_tabWidget->addTab(m_infoViewer, "Отсчеты");
    m_tabWidget->addTab(m_graphViewer, "Графики");

    m_tabWidget->setStyleSheet("color: rgb(119, 133, 255);"
                               "font-weight: 600;");
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    QPixmap background(":/src/background.png");

    background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    painter.drawPixmap(0, 0, background);

    QWidget::paintEvent(e);
}
