#include "manualtabtargets.h"

#include "src/contants.h"
#include "src/featuremanagment/targetdesignationmodel.h"

#include <QDir>
#include <QLabel>
#include <QPainter>
#include <QBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QRandomGenerator>

using namespace View;

ManualTabTargets::ManualTabTargets(QWidget *parent)
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
    init();

    m_testButton->setText("Тест");
    m_clearAll->setText("Очистить");
    m_addButton->setText("Добавить");
    m_removeButton->setText("Удалить");

    m_tableView->setModel(m_model);
    m_tableView->setColumnWidth(0, 130);
    m_tableView->setColumnWidth(1, 150);
    m_tableView->setFixedSize(300, 220);
    m_tableView->move(10, 10);

    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(m_addButton,
            &QPushButton::clicked,
            this,
            &ManualTabTargets::onAddCoordinates);

    connect(m_removeButton,
            &QPushButton::clicked,
            this,
            &ManualTabTargets::onRemoveCoordinates);

    connect(m_testButton,
            &QPushButton::clicked,
            this,
            &ManualTabTargets::onTestCreating);

    connect(m_clearAll,
            &QPushButton::clicked,
            this,
            &ManualTabTargets::onClearAllCoordinates);
}

void ManualTabTargets::init()
{
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(10);
    gridLayout->setContentsMargins(340, 30, 30, 60);

    setStyleSheet("QLabel { background: transparent; }");

    gridLayout->addWidget(new QLabel("Азимут"), 0, 0);
    gridLayout->addWidget(m_currentAzimut, 0, 1);
    gridLayout->addWidget(new QLabel("Угол места"), 1, 0);
    gridLayout->addWidget(m_currentBeanAzimut, 1, 1);

    QGridLayout *gridLayoutBtns = new QGridLayout();

    gridLayoutBtns->addWidget(m_removeButton, 0, 0);
    gridLayoutBtns->addWidget(m_addButton, 0, 1);
    gridLayoutBtns->addWidget(m_clearAll, 1, 0);
    gridLayoutBtns->addWidget(m_testButton, 1, 1);

    gridLayout->addLayout(gridLayoutBtns, 2, 0, 1, 2);
}
const QVector<TargetDesignation> &ManualTabTargets::coordinates() const
{
    return m_model->coordinates();
}

int ManualTabTargets::countCoordinates() const
{
    return m_model->countCoordinates();
}

void ManualTabTargets::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.fillRect(rect(), QColor("#F2F0F0"));
}

void ManualTabTargets::onAddCoordinates()
{
    auto currentAzimut = fromStrToInt(m_currentAzimut->text());
    auto currentElevationAngle = fromStrToInt(m_currentBeanAzimut->text());

    m_model->append(currentAzimut, currentElevationAngle);
}

void ManualTabTargets::onRemoveCoordinates()
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

void ManualTabTargets::onTestCreating()
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
        const int maxAngle = 899;

        double azimuthStep = static_cast<double>(maxAzimuth) / count;

        for (int i = 0; i < count; i++) {
            int azimuth = static_cast<int>(i * azimuthStep) % (maxAzimuth + 1);
            int angle = (i * 19) % (maxAngle + 1);

            m_model->append(azimuth, angle);
        }
    }
}
void ManualTabTargets::onClearAllCoordinates()
{
    m_model->clear();
}
