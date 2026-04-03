#include "targetdesignationwidget.h"

#include <src/contants.h>
#include "datetarget.h"
#include "generalinfotarget.h"
#include "targetdesignations.h"

#include <QTimer>
#include <QPainter>
#include <QMessageBox>
#include <QRandomGenerator>
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
    , m_retransmitTimer(new QTimer(this))
    , m_isTestModeActive(false)
{
    initUI();

    connect(m_retransmitTimer, &QTimer::timeout, this, &TargetDesignationWidget::retransmitTargets);


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
        if (m_isTestModeActive) {
            // Если уже активно - останавливаем
            stopTestMode();
        } else {
            // Запускаем режим тестирования
            startTestMode();
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

void TargetDesignationWidget::startTestMode()
{
    m_isTestModeActive = true;
    m_testButton->setText(" Остановить отправку ");
    m_testButton->setStyleSheet("background-color: #CEA898;"
                                "font-size: 20px;");

    // Сохраняем исходные даты
    m_originalStartDate = m_dates->dates().start;
    m_originalEndDate = m_dates->dates().end;

    // Первая отправка
    sendAllTargets();

    // Запускаем таймер для проверки каждую секунду
    m_retransmitTimer->start(1000); // Проверяем каждую секунду
}

void TargetDesignationWidget::stopTestMode()
{
    m_isTestModeActive = false;
    m_retransmitTimer->stop();
    m_testButton->setText(" Множественное создание ");
    m_testButton->setStyleSheet("");

    qInfo() << "Режим автоматической отправки остановлен";
}


void TargetDesignationWidget::sendAllTargets()
{
    auto endDate = QDateTime::currentDateTime().addSecs(125);
    auto startDate = QDateTime::currentDateTime().addSecs(5);

    Application::TargetDesignations target;

    target.centerFrequency = m_generalInfo->freq();
    target.spacecraftNumber = m_generalInfo->spacecraftNumber();

    int azimut = 0;
    int el = 0;

    for (int i = 0; i < 12; i++) {

        target.directionOfPolarizaion = 1;
        target.channelNumber = i+ 1;
        target.planStartTime = fromDateToDouble(startDate);
        target.planEndTime = fromDateToDouble(endDate);

        target.count = 120;
        target.coordinates = new qint16*[target.count];

        if (i == 4) {
            azimut = 180;
            el = 20;
        } else if (i == 8) {
            azimut = 270;
            el = 40;
        }

        for(quint16 i = 0; i < target.count; i++) {
            target.coordinates[i] = new qint16[2];
            target.coordinates[i][0] = azimut * 10;
            target.coordinates[i][1] = el * 10;
        }
        emit createTargetTest(target);
    }

    m_lastDate = endDate;

    qInfo() << "Все 12 каналов отправлены";
}

void TargetDesignationWidget::retransmitTargets()
{
    if (!m_isTestModeActive)
        return;

    QDateTime currentDateTime = QDateTime::currentDateTime();

    qint64 secondsRemaining = currentDateTime.secsTo(m_lastDate);

    if (secondsRemaining <= 60 && secondsRemaining > 0) {
        qInfo() << "До окончания плана осталось" << secondsRemaining
                << "секунд. Выполняем переотправку...";

        // Переотправляем целеуказания
        sendAllTargets();

        // Обновляем дату начала на текущее время
        QDateTime newStartDate = currentDateTime;
        QDateTime newEndDate = m_originalEndDate;

        // Проверяем, не нужно ли продлить план
        if (newStartDate >= newEndDate) {
            // Если план уже закончился, продлеваем его на исходную длительность
            qint64 originalDuration = m_originalStartDate.secsTo(m_originalEndDate);
            newEndDate = newStartDate.addSecs(originalDuration);

            qInfo() << "План истек. Продлеваем до:" << newEndDate.toString("dd.MM.yyyy HH:mm:ss");
        }

        // Обновляем даты в m_dates
        Dates newDates;
        newDates.start = newStartDate;
        newDates.end = newEndDate;
        m_dates->setDates(newDates);

        qInfo() << "Дата начала обновлена:" << newStartDate.toString("dd.MM.yyyy HH:mm:ss");
        qInfo() << "Дата окончания:" << newEndDate.toString("dd.MM.yyyy HH:mm:ss");
    }
    else if (secondsRemaining <= 0) {
        // Если план уже завершился, автоматически продлеваем
        qInfo() << "План завершен. Автоматическое продление...";

        QDateTime newStartDate = currentDateTime;
        qint64 originalDuration = m_originalStartDate.secsTo(m_originalEndDate);
        QDateTime newEndDate = newStartDate.addSecs(originalDuration);

        Dates newDates;
        newDates.start = newStartDate;
        newDates.end = newEndDate;
        m_dates->setDates(newDates);

        // Отправляем сразу после продления
        sendAllTargets();

        qInfo() << "План продлен до:" << newEndDate.toString("dd.MM.yyyy HH:mm:ss");
    }
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

