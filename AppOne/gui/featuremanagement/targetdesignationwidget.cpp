#include "targetdesignationwidget.h"

#include <src/contants.h>
#include "datetarget.h"
#include "generalinfotarget.h"
#include "targetdesignations.h"

#include <QPainter>
#include <QMessageBox>
#include <QStandardItemModel>

using namespace View;

TargetDesignationWidget::TargetDesignationWidget(QWidget *parent)
    : QWidget(parent)
    , m_exit(new QPushButton(this))
    , m_create(new QPushButton(this))
    , m_testButton(new QPushButton(this))
    , m_dates(new DateTarget(this))
    , m_targets(new TargetDesignations(this))
    , m_generalInfo(new GeneralInfoTarget(this))
{
    initUI();

    connect(m_exit,
            &QPushButton::clicked,
            this,
            &TargetDesignationWidget::goBack);

    connect(m_create,
            &QPushButton::clicked,
            this,
            &TargetDesignationWidget::onTargetSend);

    connect(m_targets,
            &TargetDesignations::dateGeting,
            [this](){
                m_targets->setDates(m_dates->dates());
            });

    connect(m_testButton, &QPushButton::clicked, [this](){
        auto endDate = m_dates->dates().end;
        auto startDate = m_dates->dates().start;

        Application::TargetDesignations target;

        target.centerFrequency = m_generalInfo->freq();
        target.spacecraftNumber = m_generalInfo->spacecraftNumber();

        for (int sector = 0; sector < 4; sector++) {
            qint16 azimuth = 0;
            switch(sector) {
                case 0: azimuth = 0; break;
                case 1: azimuth = 90.0; break;
                case 2: azimuth = 180.0; break;
                case 3: azimuth = 270.0; break;
            }

            qint16 elevation = m_targets->angle();

            if (elevation < 0)
                elevation = 0;

            for (int polarization = 0; polarization < 2; polarization++) {
                int channel = sector * 2 + polarization;

                target.channelNumber = channel + 1;
                target.directionOfPolarizaion = polarization + 1;

                target.planStartTime = fromDateToDouble(startDate);
                target.planEndTime = fromDateToDouble(endDate);

                target.count = 2;
                target.coordinates = new qint16*[target.count];

                for(quint16 coordIndex = 0; coordIndex < target.count; coordIndex++) {
                    target.coordinates[coordIndex] = new qint16[2];
                    target.coordinates[coordIndex][0] = azimuth * 10;
                    target.coordinates[coordIndex][1] = elevation * 10;
                }

                emit createTargetTest(target);

                for(quint16 coordIndex = 0; coordIndex < target.count; coordIndex++) {
                    delete[] target.coordinates[coordIndex];
                }
                delete[] target.coordinates;
            }
        }
    });
}

void TargetDesignationWidget::initUI()
{
    m_generalInfo->setFixedSize(600, 80);
    m_generalInfo->move(50, 70);

    m_dates->setFixedSize(600, 50);
    m_dates->move(50, 160);

    m_targets->setFixedSize(600, 300);
    m_targets->move(50, 220);

    setFixedSize(Sizes::insideSize());
    setStyleSheet("background-color: #C3D7E4");
    setAttribute(Qt::WA_OpaquePaintEvent);

    m_create->move(580, 15);
    m_create->setText(" Создать ");
    m_create->setStyleSheet("background-color: #78C4BB;"
                            "font-size: 20px;");
    m_exit->move(25, 15);
    m_exit->setText(" Назад ");
    m_exit->setStyleSheet("background-color: #CEA898;"
                          "font-size: 20px;");

    m_testButton->setText(" Множественное создание ");
    m_testButton->move(475, 216);
}

void TargetDesignationWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.fillRect(rect(), QColor("#C3D7E4"));

    QFont font;
    font.setBold(true);
    font.setWeight(QFont::Medium);
    font.setPixelSize(30);
    painter.setFont(font);

    painter.drawText(QPoint(180, 40), "Передача целеуказания");

    QPen pen = QPen();
    pen.setWidth(2);
    pen.setColor("#708086");
    painter.setPen(pen);

    QLine line = QLine(0, 60, 700, 60);
    painter.drawLine(line);

    pen.setColor("black");
    painter.setPen(pen);
    font.setPixelSize(16);
    font.setBold(false);
    painter.setFont(font);

    m_testButton->raise();
    m_testButton->update();
}

void TargetDesignationWidget::onTargetSend()
{
    double freq = m_generalInfo->freq();
    int channel = m_generalInfo->channel();
    int polar = m_generalInfo->polarization() + 1;
    int spacecraft = m_generalInfo->spacecraftNumber();

    auto endDate = m_dates->dates().end;
    auto startDate = m_dates->dates().start;

    if (channel < 0 || channel > 13)
        return;

    if (!endDate.isValid() && !startDate.isValid()) {
        return;
    }

    int count = m_targets->coordinates().count();

    if (count > 65535) {
        QMessageBox::warning(this, "Ошибка",
                             QString("Количество целеуказаний превышает 65535 (%1)").arg(count));
        return;
    }

    if (startDate >= endDate) {
        QMessageBox::warning(this, "Ошибка", "Время окончания должно быть позже времени начала");
        return;
    }

    qint64 durationSec = startDate.secsTo(endDate);
    qint64 minDuration = count;

    if (durationSec < minDuration) {
        QDateTime minEndDateTime = startDate.addSecs(minDuration);

        QMessageBox::warning(this, "Ошибка",
                             QString("Длительность целеуказания слишком мала.\n"
                                     "Минимально подходящая дата окончания: %1").arg(minEndDateTime.toString("dd.MM.yyyy HH:mm:ss")));
        return;
    }

    Application::TargetDesignations target;

    target.channelNumber = channel;
    target.directionOfPolarizaion = polar;
    target.spacecraftNumber = spacecraft;
    target.centerFrequency = freq;

    target.planStartTime = fromDateToDouble(startDate);
    target.planEndTime = fromDateToDouble(endDate);

    target.count = count;
    target.coordinates = new qint16*[target.count];

    for(quint16 i = 0; i < target.count; i++) {
        target.coordinates[i] = new qint16[2];
        target.coordinates[i][0] = m_targets->coordinates()[i].azimut * 10;
        target.coordinates[i][1] = m_targets->coordinates()[i].elev * 10;
    }

    emit createTarget(target);
}

TargetDesignationWidget::~TargetDesignationWidget()
{
    delete m_exit;
    delete m_create;
}

