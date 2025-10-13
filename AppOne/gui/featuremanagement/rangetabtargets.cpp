#include "rangetabtargets.h"

#include <QLabel>
#include <QPainter>
#include <QLineEdit>
#include <QGridLayout>

#include "src/featuremanagment/targetdesignationmodel.h"

using namespace View;

RangeTabTargets::RangeTabTargets(QWidget *parent)
    : QWidget(parent)
    , m_tableView(new QTableView(this))
    , m_btnClear(new QPushButton("Очистить", this))
    , m_buttonCreate(new QPushButton("Создать", this))
    , m_step(new QLineEdit(this))
    , m_beamStart(new QLineEdit(this))
    , m_azimutStart(new QLineEdit(this))
    , m_increase(new QComboBox(this))
    , m_model(new TargetDesignationModel(this))

{
    init();

    m_increase->addItem("Азимут");
    m_increase->addItem("Угол места");
    m_increase->addItem("Совместно");

    connect(m_buttonCreate, &QPushButton::clicked,
            this, &RangeTabTargets::onButtonCreate);

    connect(m_btnClear, &QPushButton::clicked, [this](){
        m_model->clear();
    });

    Sector sector_1;
    sector_1.start = 0;
    sector_1.end = 899;

    Sector sector_2;
    sector_2.start = 900;
    sector_2.end = 1799;

    Sector sector_3;
    sector_3.start = 1800;
    sector_3.end = 2699;

    Sector sector_4;
    sector_4.start = 2700;
    sector_4.end = 3599;

    m_sectors.push_back(sector_1);
    m_sectors.push_back(sector_2);
    m_sectors.push_back(sector_3);
    m_sectors.push_back(sector_4);
}

void RangeTabTargets::onButtonCreate()
{
    m_model->clear();

    if (m_azimutStart->text() != "" && m_step->text() != "" &&
        m_beamStart->text() != "") {

        double azStart = m_azimutStart->text().toDouble();
        double beamStart = m_beamStart->text().toDouble();
        double step = m_step->text().toDouble();
        QString increaseMode = m_increase->currentText();

        // Находим сектор для начального азимута
        Sector targetSector;
        for (const Sector& sector : m_sectors) {
            if (azStart >= sector.start && azStart <= sector.end) {
                targetSector = sector;
                break;
            }
        }

        if (increaseMode == "Азимут") {
            // Увеличиваем только азимут
            for (double az = azStart; az <= targetSector.end; az += step) {
                m_model->append(az, beamStart);
            }
        }
        else if (increaseMode == "Угол места") {
            // Увеличиваем только угол места (в диапазоне 0-899)
            for (double beam = beamStart; beam <= 899; beam += step) {
                m_model->append(azStart, beam);
            }
        }
        else if (increaseMode == "Совместно") {
            // Увеличиваем одновременно азимут и угол места
            double az = azStart;
            double beam = beamStart;

            while (az <= targetSector.end && beam <= 899) {
                m_model->append(az, beam);
                az += step;
                beam += step;
            }
        }
    }
}

QVector<TargetDesignation> RangeTabTargets::coordinates()
{
    return m_model->coordinates();
}

void RangeTabTargets::init()
{
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setContentsMargins(240, 0, 30, 0);

    setStyleSheet("QLabel { background: transparent; }");

    QGridLayout *gridLayout_2 = new QGridLayout();
    gridLayout->addLayout(gridLayout_2, 0, 0, 1, 4);


    gridLayout_2->addWidget(m_increase, 0, 0);
    gridLayout_2->addWidget(new QLabel("Шаг"), 0, 1);
    gridLayout_2->addWidget(m_step, 0, 2);

    gridLayout_2 ->addWidget(new QLabel("Азимут"), 1, 0);
    gridLayout_2 ->addWidget(m_azimutStart, 1, 1);

    gridLayout_2 ->addWidget(new QLabel("Угол места"), 2, 0);
    gridLayout_2 ->addWidget(m_beamStart, 2, 1);

    m_azimutStart->setFixedWidth(50);
    m_beamStart->setFixedWidth(50);

    m_tableView->setModel(m_model);
    m_tableView->setColumnWidth(0, 90);
    m_tableView->setColumnWidth(1, 90);
    m_tableView->setFixedSize(200, 220);
    m_tableView->move(10, 10);

    m_buttonCreate->setFixedWidth(80);
    gridLayout->addWidget(m_btnClear, 3, 0, 1, 2, Qt::AlignRight);
    gridLayout->addWidget(m_buttonCreate, 3, 2, 1, 2, Qt::AlignLeft);

    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
}

void RangeTabTargets::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.fillRect(rect(), QColor("#F2F0F0"));
}
