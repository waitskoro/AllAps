#include "suntabtargets.h"

#include <QIcon>
#include <QLabel>
#include <QPainter>
#include <QDateTime>
#include <QLineEdit>
#include <QGridLayout>

#include "coordinatesconverter.h"
#include "src/featuremanagment/targetdesignationmodel.h"

using namespace View;

SunTabTargets::SunTabTargets(TargetDesignationModel *model, QWidget *parent)
    : QWidget(parent)
    , m_model(model)
    , m_tableView(new QTableView(this))
    , m_lat(new QLineEdit(this))
    , m_lon(new QLineEdit(this))
    , m_lonConverter(new QPushButton(this))
    , m_latConverter(new QPushButton(this))
    , m_btnClear(new QPushButton("Очистить", this))
    , m_buttonCreate(new QPushButton("Создать", this))
    , m_coordsConverter(new CoordinatesConverter())
    , m_sunTargetParser(new SunTargetDesignations(this))
{
    init();

    connect(m_lonConverter, &QPushButton::clicked, [this](bool) {
        m_coordsConverter->showWindow(CoordinatesConverter::Type::Lon);
    });

    connect(m_latConverter, &QPushButton::clicked, [this](bool) {
        m_coordsConverter->showWindow(CoordinatesConverter::Type::Lat);
    });

    connect(m_coordsConverter, &CoordinatesConverter::converted, [this](double val, CoordinatesConverter::Type type) {
        if (type == CoordinatesConverter::Type::Lat)
            m_lat->setText(QString::number(val));

        if (type == CoordinatesConverter::Type::Lon)
            m_lon->setText(QString::number(val));
    });

    connect(m_buttonCreate, &QPushButton::clicked, [this](bool){
        emit dateGeting();
    });

    connect(m_btnClear, &QPushButton::clicked, [this](bool){
        m_model->clear();
    });
}

QVector<TargetDesignation> SunTabTargets::coordinates()
{
    return m_model->coordinates();
}

void SunTabTargets::init()
{
    m_latConverter->setFixedWidth(20);
    m_lonConverter->setFixedWidth(20);

    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setContentsMargins(240, 0, 30, 0);

    setStyleSheet("QLabel { background: transparent; }");

    QGridLayout *gridLayout_2 = new QGridLayout();
    gridLayout->addLayout(gridLayout_2, 0, 0, 1, 4);

    gridLayout_2->addWidget(new QLabel("Широта"), 0, 0);
    gridLayout_2->addWidget(m_lat, 0, 1);
    gridLayout_2->addWidget(m_latConverter, 0, 2);
    gridLayout_2->addWidget(new QLabel("Долгота"), 0, 3);
    gridLayout_2->addWidget(m_lon, 0, 4);
    gridLayout_2->addWidget(m_lonConverter, 0, 5);

    gridLayout->addWidget(m_btnClear, 3, 0, 1, 2, Qt::AlignRight);
    gridLayout->addWidget(m_buttonCreate, 3, 2, 1, 2, Qt::AlignLeft);

    m_tableView->setModel(m_model);
    m_tableView->setColumnWidth(0, 90);
    m_tableView->setColumnWidth(1, 90);
    m_tableView->setFixedSize(200, 220);
    m_tableView->move(10, 10);

    m_buttonCreate->setFixedWidth(80);
    m_lonConverter->setFixedWidth(25);
    m_latConverter->setFixedWidth(25);

    m_lonConverter->setIcon(QIcon(":/gui/src/calculator.png"));
    m_latConverter->setIcon(QIcon(":/gui/src/calculator.png"));
    m_lonConverter->setIconSize(QSize(16, 16));
    m_latConverter->setIconSize(QSize(16, 16));

    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
}

void SunTabTargets::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.fillRect(rect(), QColor("#F2F0F0"));
}

void SunTabTargets::onButtonCreate(Dates dates)
{
    if (m_lat->text() != "" && m_lon->text() != "") {

        auto lat = m_lat->text().toDouble();
        auto lon = m_lon->text().toDouble();

        Coordinates *coords = new Coordinates(lat, lon);

        m_sunTargetParser->setCoords(*coords);
        auto targets = m_sunTargetParser->getTargets(dates.start, dates.end);

        for (int i = 0; i < targets.count(); i++) {
            m_model->append(targets[i].azimut / 10.0, targets[i].angle / 10.0);
        }
    }
}
