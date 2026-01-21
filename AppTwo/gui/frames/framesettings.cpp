#include "framesettings.h"
#include "ui_framesettings.h"

FrameSettings::FrameSettings(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::FrameSettings)
{
    ui->setupUi(this);

    connect(ui->pushButtonVisible, &QPushButton::clicked, [this]() {
        bool isVisible = !ui->widget->isVisible();
        ui->widget->setVisible(isVisible);

        if (isVisible) {
            ui->pushButtonVisible->setText("<<");
            setMaximumWidth(285);
        } else {
            ui->pushButtonVisible->setText(">>");
            setMaximumWidth(45);
        }
    });

    connectSettings();
}

FrameSettings::~FrameSettings()
{
    delete ui;
}

double FrameSettings::minLevel()
{
    return ui->minLevel->value();
}

double FrameSettings::maxLevel()
{
    return ui->maxLevel->value();
}

int FrameSettings::historySize()
{
    return ui->spectrogramHistory->value();
}

void FrameSettings::connectSettings()
{
    connect(ui->checkGraphI, &QCheckBox::clicked,
            this, &FrameSettings::checkGraghIChanged);

    connect(ui->checkGraphQ, &QCheckBox::clicked,
            this, &FrameSettings::checkGraghQChanged);

    connect(ui->checkGraphPower, &QCheckBox::clicked,
            this, &FrameSettings::checkGraghPowerChanged);

    connect(ui->checkGraphSpector, &QCheckBox::clicked,
            this, &FrameSettings::checkGraghSpectorChanged);

    connect(ui->checkSpectrogram, &QCheckBox::clicked,
            this, &FrameSettings::checkSpectrogramChanged);

    // =========================================================================

    connect(ui->spinPowerRange, &QSpinBox::valueChanged,
            this, &FrameSettings::powerRangeChanged);

    // =========================================================================

    connect(ui->minLevel, &QDoubleSpinBox::valueChanged,
            this, &FrameSettings::minLevelChanged);

    connect(ui->maxLevel, &QDoubleSpinBox::valueChanged,
            this, &FrameSettings::maxLevelChanged);

    connect(ui->spectrogramHistory, &QSpinBox::valueChanged,
            this, &FrameSettings::spectrogramHistoryChanged);

    connect(ui->colorSheme, &QComboBox::currentTextChanged,
            this, &FrameSettings::colorSchemeChanged);
}
