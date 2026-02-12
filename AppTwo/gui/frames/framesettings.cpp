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
