#include "targetdesignationwidget.h"

#include <src/contants.h>
#include <gui/featuremanagement/targetdesignationtable.h>

#include <QPainter>
#include <QMessageBox>
#include <QStandardItemModel>

using namespace View;

TargetDesignationWidget::TargetDesignationWidget(QWidget *parent)
    : QWidget(parent)
    , m_exit(new QPushButton(this))
    , m_create(new QPushButton(this))
    , m_numberChannelData(new QComboBox(this))
    , m_spacecraftNumber(new QLineEdit(this))
    , m_centerFrequency(new QLineEdit(this))
    , m_startDate(new QDateEdit(this))
    , m_startTime(new QTimeEdit(this))
    , m_endDate(new QDateEdit(this))
    , m_endTime(new QTimeEdit(this))
    , m_coordinatesView(new TargetDesignationTable(this))
    , m_polarization(new QComboBox(this))
{
    setFixedSize(Sizes::insideSize());
    setStyleSheet("background-color: #C3D7E4");
    setAttribute(Qt::WA_OpaquePaintEvent);

    initUI();

    for (int i = 0; i < 12; i++)
        m_numberChannelData->addItem(QString::number(i + 1), QVariant(i));

    m_polarization->addItem("Правая круговая", QVariant(0));
    m_polarization->addItem("Левая круговая", QVariant(1));
    m_polarization->addItem("Вертикальная", QVariant(2));
    m_polarization->addItem("Горизонтальная", QVariant(3));
    m_polarization->addItem("Линейная +45%", QVariant(4));
    m_polarization->addItem("Линейная -45%", QVariant(5));

    m_startDate->setDisplayFormat("MM/dd/yy");
    m_startTime->setDisplayFormat("hh:mm:ss");
    m_endDate->setDisplayFormat("MM/dd/yy");
    m_endTime->setDisplayFormat("hh:mm:ss");

    m_startDate->setDate(QDate::currentDate());
    m_startTime->setTime(QTime::currentTime());
    m_endDate->setDate(QDate::currentDate());
    m_endTime->setTime(QTime::currentTime().addSecs(3600));

    connect(m_exit,
            &QPushButton::clicked,
            this,
            &TargetDesignationWidget::goBack);

    connect(m_create,
            &QPushButton::clicked,
            this,
            &TargetDesignationWidget::onTargetSend);
}

void TargetDesignationWidget::initUI()
{
    m_create->move(580, 15);
    m_create->setText(" Создать ");
    m_create->setStyleSheet("background-color: #78C4BB;"
                            "font-size: 20px;");
    m_exit->move(25, 15);
    m_exit->setText(" Назад ");
    m_exit->setStyleSheet("background-color: #CEA898;"
                          "font-size: 20px;");

    m_numberChannelData->setFixedSize(100, 25);
    m_numberChannelData->move(235, 90);

    m_spacecraftNumber->setFixedSize(100, 25);
    m_spacecraftNumber->move(235, 130);

    m_centerFrequency->setFixedSize(80, 25);
    m_centerFrequency->move(560, 90);

    m_coordinatesView->move(50, 280);

    m_polarization->move(495, 130);
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

    painter.drawText(QPoint(180, 40),"Передача целеуказания");

    QPen pen = QPen();
    pen.setWidth(2);
    pen.setColor("#708086");
    painter.setPen(pen);

    QLine line = QLine(0, 60, 700, 60);
    painter.drawLine(line);

    QRect rect = QRect(50, 80, 600, 100);
    painter.drawRect(rect);
    painter.fillRect(rect, QColor("#F2F0F0"));

    QRect rect2 = QRect(50, 205, 600, 50);
    painter.drawRect(rect2);
    painter.fillRect(rect2, QColor("#F2F0F0"));

    pen.setColor("black");
    painter.setPen(pen);
    font.setPixelSize(16);
    font.setBold(false);
    painter.setFont(font);

    painter.drawText(QPoint(60, 110),"Номер канала данных");
    painter.drawText(QPoint(60, 150),"Номер КА в системе");
    painter.drawText(QPoint(350, 110),"Центральная частота кГц");

    painter.drawText(QPoint(350, 150),"Поларизация");

    m_startDate->move(170, 215);
    m_startDate->setFixedSize(70, 25);
    m_startDate->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_startTime->move(245, 215);
    m_startTime->setFixedSize(70, 25);
    m_startTime->setButtonSymbols(QAbstractSpinBox::NoButtons);

    m_endDate->move(470, 215);
    m_endDate->setFixedSize(70, 25);
    m_endDate->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_endTime->move(545, 215);
    m_endTime->setFixedSize(70, 25);
    m_endTime->setButtonSymbols(QAbstractSpinBox::NoButtons);

    painter.drawText(QPoint(60, 235), "Начало плана");
    painter.drawText(QPoint(330, 235), "Окончание плана");
}

void TargetDesignationWidget::onTargetSend()
{
    if (m_numberChannelData->currentText() == "") {
        return;
    }

    if (m_spacecraftNumber->text() == "") {
        return;
    }

    if (m_centerFrequency->text() == "") {
        return;
    }
    if (!m_startDate->date().isValid() && !m_startTime->time().isValid()) {
        return;
    }

    if (!m_endDate->date().isValid() && !m_endTime->time().isValid()) {
        return;
    }

    int targetCount = m_coordinatesView->countCoordinates();

    if (targetCount > 65535) {
        QMessageBox::warning(this, "Ошибка",
                             QString("Количество значений целеуказаний превышает 65535 (%1)").arg(targetCount));
        return;
    }

    QDateTime startDateTime(m_startDate->date(), m_startTime->time());
    QDateTime endDateTime(m_endDate->date(), m_endTime->time());

    if (startDateTime >= endDateTime) {
        QMessageBox::warning(this, "Ошибка", "Время окончания должно быть позже времени начала");
        return;
    }

    qint64 durationSec = startDateTime.secsTo(endDateTime);
    qint64 minDuration = targetCount;

    if (durationSec < minDuration) {
        QDateTime minEndDateTime = startDateTime.addSecs(minDuration);

        QMessageBox::warning(this, "Ошибка",
                             QString("Длительность целеуказания слишком мала.\n"
                                     "Минимально подходящая дата окончания: %1").arg(minEndDateTime.toString("dd.MM.yyyy HH:mm:ss")));
        return;
    }

    const auto& coordinates = m_coordinatesView->coordinates();

    Application::TargetDesignations target;

    target.channelNumber = toDouble(m_numberChannelData->currentText());
    target.directionOfPolarizaion = m_polarization->currentIndex();
    target.spacecraftNumber = toDouble(m_spacecraftNumber->text());
    target.centerFrequency = toDouble(m_centerFrequency->text());

    target.planStartTime = fromDateToDouble(startDateTime);
    target.planEndTime = fromDateToDouble(endDateTime);

    target.count = targetCount;
    target.coordinates = new qint16*[target.count];

    for(quint16 i = 0; i < target.count; i++) {
        target.coordinates[i] = new qint16[2];
        target.coordinates[i][0] = coordinates[i][0];
        target.coordinates[i][1] = coordinates[i][1];
    }

    emit createTarget(target);
}

TargetDesignationWidget::~TargetDesignationWidget()
{
    delete m_exit;
    delete m_create;
    delete m_numberChannelData;
    delete m_spacecraftNumber;
    delete m_startDate;
    delete m_startTime;
    delete m_endDate;
    delete m_endTime;
    delete m_coordinatesView;
}

