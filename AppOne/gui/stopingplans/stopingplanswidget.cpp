#include "stopingplanswidget.h"
#include "src/contants.h"
#include <QPainter>
#include <QHBoxLayout>

using namespace View;

StopingPlansWidget::StopingPlansWidget(QWidget *parent)
    : QWidget(parent)
    , m_label(new QLabel(this))
    , m_insideRect(new QRect(250, 150, 300, 200))
    , m_cancel(new QPushButton("Cancel", this))
    , m_buttonOk(new QPushButton("OK", this))
{
    close();

    setFixedSize(Sizes::mainSize());

    connect(m_cancel,
            &QPushButton::clicked,
            [this](bool){
                close();
                emit canceled();
            });

    connect(m_buttonOk,
            &QPushButton::clicked,
            [this](bool){
                if (!m_isStoped) {
                    emit send();

                    QString text = "Ожидания ответа от программы на\n"
                                   "остановку приема";
                    m_label->setText(text);

                    m_buttonOk->setVisible(false);
                } else {
                    close();
                    emit stoped();
                }
            });

    m_label->move(250, 150);
    m_label->setFixedSize(300, 150);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setStyleSheet("background-color: transparent");
}

void StopingPlansWidget::showWindow()
{
    show();
    m_isStoped = false;

    QString text = "Вы уверены, что хотите совершить\n"
                   "остановку приема по всем каналам\n"
                   "данных?";
    m_label->setText(text);

    m_buttonOk->setVisible(true);
    m_cancel->setVisible(true);

    m_cancel->move(m_insideRect->left() + 50, m_insideRect->bottom() - 50);
    m_buttonOk->move(m_insideRect->right() - 50 - 80, m_insideRect->bottom() - 50);

}

void StopingPlansWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPainter painter(this);
    painter.fillRect(rect(), QColor(217, 217, 217, 127));

    painter.drawRect(*m_insideRect);
    painter.fillRect(*m_insideRect, QColor(136, 194, 230));
}

void StopingPlansWidget::messageRecivied(ExecutedTheCommand result)
{
    QString text;

    if (result.result == 0) {
        text = "Успешное завершение приема по\n"
               "всем каналам. Переход в режим\n"
               "ожидания новых планов слежения";
    } else {
        text = QString("Результат исполнения команды: %1").arg(result.result);
    }

    m_buttonOk->setVisible(true);
    m_cancel->setVisible(false);

    int buttonY = m_insideRect->bottom() - 50;
    m_buttonOk->move(m_insideRect->left() + 110, buttonY);

    m_label->setText(text);

    m_isStoped = true;
}
