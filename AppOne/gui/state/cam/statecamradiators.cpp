#include "statecamradiators.h"

#include "camradiatorslist.h"
#include "src/statemessage.h"

#include <QPainter>

using namespace View;

StateCamRadiators::StateCamRadiators(QWidget *parent)
    : QWidget(parent)
    , m_list(new CamRadiatorsList(this))
{
    setWindowFlags(Qt::Popup);
    setFixedSize(410, 250);

    m_list->move(25, 10);
    m_list->setFixedSize(rect().width() - 30, rect().bottom() - 20);
}

void StateCamRadiators::setInfo(qint32 count, QVariant &var)
{
    std::vector<EmmiterState> values;
    EmmiterState* vectorData = var.value<EmmiterState*>();

    for (int i = 0; i < count; i++) {
        m_list->addRadiators(&vectorData[i]);
    }
}

void StateCamRadiators::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.fillRect(rect(), "#36E8D3");

    QFont font;
    font.setBold(true);
    font.setWeight(QFont::DemiBold);
    font.setPixelSize(18);
    painter.setFont(font);

    painter.setPen("#3C388D");

    QString text = "Состояния радиаторов";
    QFontMetrics fm(painter.font());
    int textWidth = fm.horizontalAdvance(text);

    painter.save();

    painter.translate(0, height() / 2);

    painter.rotate(-90);

    painter.drawText(-textWidth / 2, 18, text);

    painter.restore();
}
