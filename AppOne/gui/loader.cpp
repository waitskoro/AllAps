#include "loader.h"

#include <QTimer>
#include <QPainter>

#include "src/contants.h"

using namespace View;

Loader::Loader(const QString &text, const QRect &rect, QWidget *parent)
    : QWidget(parent)
    , m_mainRect(QRect(0, 0, 250, 150))
    , m_rect(rect)
    , m_cancel(new QPushButton())
    , m_headerText(text)
    , m_dotCount(0)
    , m_baseText(text)
{
    setFixedSize(rect.size());
    m_mainRect.moveTo(centeringCoordinates(rect.size(),
                                           m_mainRect.size()));

    m_cancel = createButton(this, "Отмена", 100, 30);
    m_cancel->move(
        m_mainRect.x() + centerX(m_mainRect.size(), m_cancel->size()),
        m_mainRect.y() + 110
        );

    m_dotsTimer = new QTimer(this);
    connect(m_dotsTimer, &QTimer::timeout, this, &Loader::updateDots);

    connect(m_cancel,
            &QPushButton::clicked,
            this,
            &Loader::onCanceling);
}

void Loader::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPainter painter(this);
    painter.fillRect(m_rect,
                     QColor(217, 217, 217, 127));

    QFont font;
    font.setBold(true);
    font.setWeight(QFont::Medium);
    font.setPixelSize(20);
    painter.setFont(font);

    painter.fillRect(m_mainRect,
                     "#88C2E6");
    painter.drawRect(m_mainRect);

    int textWidth = painter.fontMetrics().horizontalAdvance(m_headerText);
    int xPos = m_mainRect.x() + (m_mainRect.width() - textWidth) / 2;

    painter.drawText(xPos,
                     m_mainRect.y() + 60,
                     m_headerText);
}
void Loader::start()
{
    m_dotCount = 0;
    m_dotsTimer->start(500);
    show();
}

void Loader::stop()
{
    m_dotsTimer->stop();
    close();
}

void Loader::updateDots()
{
    m_dotCount++;
    if (m_dotCount > 3) {
        m_dotCount = 0;
    }

    m_headerText = m_baseText;
    for (int i = 0; i < m_dotCount; i++) {
        m_headerText += ".";
    }

    update();
}

void Loader::onCanceling()
{
    emit cancelConnecting();
    stop();
}
