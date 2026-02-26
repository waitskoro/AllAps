#include "framesettings.h"

FrameSettings::FrameSettings(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::FrameSettings)
{
    ui->setupUi(this);

    connectSettings();
}

FrameSettings::~FrameSettings()
{
    delete ui;
}

void FrameSettings::connectSettings()
{
    connect(ui->pushButtonVisible, &QPushButton::clicked,
            this, &FrameSettings::onButtonVisibleClicked);

    // =========================================================================

    connect(ui->checkGraphI, &QCheckBox::clicked,
            this, &FrameSettings::checkGraghIChanged);

    connect(ui->checkGraphQ, &QCheckBox::clicked,
            this, &FrameSettings::checkGraghQChanged);

    connect(ui->checkGraphPower, &QCheckBox::clicked,
            this, &FrameSettings::checkGraghPowerChanged);

    connect(ui->checkGraphSpector, &QCheckBox::clicked,
            this, &FrameSettings::checkGraghSpectorChanged);

    // =========================================================================

    connect(ui->spinPowerRange, &QSpinBox::valueChanged,
            this, &FrameSettings::powerRangeChanged);

    // =========================================================================

    connect(ui->spectrogramBtn, &QPushButton::clicked,
            this, &FrameSettings::spectrogramView);
}

void FrameSettings::onButtonVisibleClicked()
{
    bool isVisible = !ui->widget->isVisible();
    ui->widget->setVisible(isVisible);

    if (isVisible) {
        ui->pushButtonVisible->setText("<<");
        setMaximumWidth(285);
    } else {
        ui->pushButtonVisible->setText(">>");
        setMaximumWidth(45);
    }
}
