#include "targetdesignationtable.h"

#include "src/contants.h"
#include "src/featuremanagment/targetdesignationmodel.h"

#include <QDir>
#include <QPainter>
#include <QMessageBox>
#include <QInputDialog>
#include <QRandomGenerator>

using namespace View;

TargetDesignationTable::TargetDesignationTable(QWidget *parent)
    : QWidget(parent)
    , m_tableView(new QTableView(this))
    , m_currentAzimut(new QLineEdit(this))
    , m_currentBeanAzimut(new QLineEdit(this))
    , m_addButton(new QPushButton(this))
    , m_removeButton(new QPushButton(this))
    , m_testButton(new QPushButton(this))
    , m_clearAll(new QPushButton(this))
    , m_model(new TargetDesignationModel(this))
{
    setFixedSize(600, 200);

    m_currentAzimut->setFixedSize(70, 25);
    m_currentAzimut->move(520, 25);
    m_currentBeanAzimut->setFixedSize(70, 25);
    m_currentBeanAzimut->move(520, 55);

    m_addButton->move(470, 90);
    m_addButton->setText(" Добавить ");
    m_addButton->setFixedSize(100, 25);

    m_removeButton->move(360, 90);
    m_removeButton->setText(" Удалить ");
    m_removeButton->setFixedSize(100, 25);

    m_testButton->move(470, 140);
    m_testButton->setText(" Тест ");
    m_testButton->setFixedSize(100, 25);

    m_clearAll->move(360, 140);
    m_clearAll->setText(" Очистить ");
    m_clearAll->setFixedSize(100, 25);

    m_tableView->setModel(m_model);
    m_tableView->setColumnWidth(0, 130);
    m_tableView->setColumnWidth(1, 150);
    m_tableView->setFixedSize(300, 150);
    m_tableView->move(25, 25);

    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(m_addButton,
            &QPushButton::clicked,
            this,
            &TargetDesignationTable::onAddCoordinates);

    connect(m_removeButton,
            &QPushButton::clicked,
            this,
            &TargetDesignationTable::onRemoveCoordinates);

    connect(m_testButton,
            &QPushButton::clicked,
            this,
            &TargetDesignationTable::onTestCreating);

    connect(m_clearAll,
            &QPushButton::clicked,
            this,
            &TargetDesignationTable::onClearAllCoordinates);
}

const std::vector<std::array<short, 2>>& TargetDesignationTable::coordinates() const
{
    return m_model->coordinates();
}

int TargetDesignationTable::countCoordinates() const
{
    return m_model->countCoordinates();
}

void TargetDesignationTable::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.fillRect(rect(), QColor("#F2F0F0"));

    painter.drawText(350, 45, "Текущий азимут");
    painter.drawText(350, 75, "Текущий угол азимута");
}

void TargetDesignationTable::onAddCoordinates()
{
    auto currentAzimut = fromStrToInt(m_currentAzimut->text());
    auto currentElevationAngle = fromStrToInt(m_currentBeanAzimut->text());

    if (currentAzimut != -1 && currentElevationAngle != -1)
        m_model->append(currentAzimut, currentElevationAngle);
}

void TargetDesignationTable::onRemoveCoordinates()
{
    QModelIndexList selected = m_tableView->selectionModel()->selectedRows();
    if (selected.isEmpty())
        return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение",
                                  QString("Удалить строку?"),
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply != QMessageBox::Yes)
        return;

    for (int i = selected.count() - 1; i >= 0; --i) {
        m_model->removeRow(selected.at(i).row());
    }
}

void TargetDesignationTable::onTestCreating()
{
    QInputDialog dialog;
    dialog.setWindowTitle(tr("Укажите количество координат"));
    dialog.setLabelText(tr("Укажите количество координат, \nкоторые нужно сгенерировать:"));
    dialog.setInputMode(QInputDialog::IntInput);
    dialog.setIntMinimum(1);
    dialog.setIntMaximum(65536);

    if (dialog.exec() == QDialog::Accepted)
    {
        int count = dialog.intValue();
        count = qMin(count, 65536);

        const int maxAzimuth = 3600;
        const int maxAngle = 900;

        double azimuthStep = static_cast<double>(maxAzimuth) / count;

        for (int i = 0; i < count; i++) {
            int azimuth = static_cast<int>(i * azimuthStep) % (maxAzimuth + 1);
            int angle = (i * 19) % (maxAngle + 1);

            m_model->append(azimuth, angle);
        }
    }
}
void TargetDesignationTable::onClearAllCoordinates()
{
    m_model->clear();
}
