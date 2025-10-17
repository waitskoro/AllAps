#include "rangetabtargets.h"

#include <QLabel>
#include <QPainter>
#include <QLineEdit>
#include <QGridLayout>
#include <QSpacerItem>

#include "src/featuremanagment/targetdesignationmodel.h"

using namespace View;

RangeTabTargets::RangeTabTargets(TargetDesignationModel *model, QWidget *parent)
    : QWidget(parent)
    , m_tableView(new QTableView(this))
    , m_btnClear(new QPushButton("Очистить", this))
    , m_buttonCreate(new QPushButton("Создать", this))
    , m_step(new QDoubleSpinBox(this))
    , m_angleStart(new QDoubleSpinBox(this))
    , m_azimutStart(new QDoubleSpinBox(this))
    , m_endParam(new QDoubleSpinBox(this))
    , m_increase(new QComboBox(this))
    , m_model(model)
{
    init();

    m_increase->addItem("Азимут");
    m_increase->addItem("Угол места");

    connect(m_buttonCreate, &QPushButton::clicked,
            this, &RangeTabTargets::onButtonCreate);

    connect(m_btnClear, &QPushButton::clicked, [this](){
        m_model->clear();
    });

    connect(m_increase, &QComboBox::currentTextChanged, [this](){
        QString increaseMode = m_increase->currentText();
        if (increaseMode == "Азимут") {
            m_step->setRange(1, 360);
            m_endParam->setRange(0, 360);
        } else {
            m_step->setRange(1, 90);
            m_endParam->setRange(0, 90);
        }
    });

    m_endParam->setRange(0, 360);
    m_angleStart->setRange(0, 89.99);
    m_azimutStart->setRange(0, 359.99);

    m_step->setRange(1, 360);
}

void RangeTabTargets::onButtonCreate()
{
    m_model->clear();

    if (m_azimutStart->text() != "" && m_step->text() != "" &&
        m_angleStart->text() != "") {

        double step = m_step->value();
        double azStart = m_azimutStart->value();
        double angleStart = m_angleStart->value();
        QString increaseMode = m_increase->currentText();

        if (increaseMode == "Азимут") {
            // Увеличиваем только азимут
            for (double az = azStart; az <= m_endParam->value(); az += step) {
                m_model->append(az, angleStart);
            }
        }
        else{
            // Увеличиваем только угол места (в диапазоне 0-89.9)
            for (double angle = angleStart; angle <= m_endParam->value(); angle += step) {
                m_model->append(azStart, angle);
            }
        }
    }
}

void RangeTabTargets::init()
{
    setStyleSheet("QLabel { background: transparent; color: black;}");

    //Общий
    QHBoxLayout *gridLayout = new QHBoxLayout(this);
    gridLayout->setContentsMargins(240, 0, 30, 0);

    // Левая сторона (список указаний)
    QGridLayout *gridLayout_2 = new QGridLayout();
    gridLayout->addLayout(gridLayout_2);

    // Правая
    QVBoxLayout *gridLayout_3 = new QVBoxLayout();
    gridLayout->addLayout(gridLayout_3);

    //=============================================
    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    gridLayout_3->addLayout(hBoxLayout);

    QHBoxLayout *hBoxLayout_2 = new QHBoxLayout();
    gridLayout_3->addLayout(hBoxLayout_2);

    QHBoxLayout *hBoxLayout_3 = new QHBoxLayout();
    gridLayout_3->addLayout(hBoxLayout_3);

    QHBoxLayout *hBoxLayout_4 = new QHBoxLayout();
    gridLayout_3->addLayout(hBoxLayout_4);

    hBoxLayout->addWidget(m_increase);
    hBoxLayout->addSpacerItem(new QSpacerItem(110, 10));
    hBoxLayout->addWidget(new QLabel("Шаг"));
    hBoxLayout->addWidget(m_step);

    hBoxLayout_2 ->addWidget(new QLabel("Конец выбранного параметра"));
    hBoxLayout_2 ->addWidget(m_endParam);

    hBoxLayout_3 ->addWidget(new QLabel("Азимут"));
    hBoxLayout_3 ->addWidget(m_azimutStart);
    hBoxLayout_3->addSpacerItem(new QSpacerItem(30, 10));
    hBoxLayout_3 ->addWidget(new QLabel("Угол места"));
    hBoxLayout_3 ->addWidget(m_angleStart);

    hBoxLayout_4->addSpacerItem(new QSpacerItem(50, 10));
    hBoxLayout_4->addWidget(m_btnClear);
    hBoxLayout_4->addWidget(m_buttonCreate);
    hBoxLayout_4->addSpacerItem(new QSpacerItem(50, 10));

    m_tableView->setModel(m_model);
    m_tableView->setColumnWidth(0, 90);
    m_tableView->setColumnWidth(1, 90);
    m_tableView->setFixedSize(200, 220);
    m_tableView->move(10, 10);

    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
}

void RangeTabTargets::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.fillRect(rect(), QColor("#F2F0F0"));
}
