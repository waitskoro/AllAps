#include "stateworkingwidget.h"

#include <src/contants.h>
#include "stateworkinglist.h"
#include "cam/statecamlist.h"

#include <QPainter>

using namespace View;

StateWorkingWidget::StateWorkingWidget(QWidget *parent)
    : QWidget(parent)
    , m_label(new QLabel(this))
    , m_camNumber(new QLabel(this))
    , m_exit(new QPushButton(this))
    , m_headerLabel(new QLabel(this))
    , m_stateWorkingList(new StateWorkingList(this))
    , m_camList(new StateCamList(this))
{
    initUI();

    setFixedSize(Sizes::insideSize());

    m_label->setFixedSize(Sizes::insideSize());
    m_label->setAttribute(Qt::WA_AlwaysStackOnTop);

    connect(m_exit,
            &QPushButton::clicked,
            this,
            &StateWorkingWidget::exit);

    m_label->show();
    m_camList->hide();
    m_camNumber->hide();
    m_stateWorkingList->hide();

    m_stateWorkingList->move(10, 245);

    m_camList->hide();
    m_camList->move(420, 265);

    m_camNumber->move(440, 240);
    m_camNumber->setFixedWidth(300);

    m_camNumber->setStyleSheet("background-color: transparent;"
                               "font-size: 16px");

    connect(m_stateWorkingList,
            &StateWorkingList::itemClicked,
            [this](std::vector<CamState> data, int sector){
                m_camList->clear();

                for (auto& cam : data) {
                    m_camList->addCam(cam);
                }                

                m_camNumber->setText(QString("Сектор AC: %1").arg(sector + 1));

            });
}

void StateWorkingWidget::addState(StateMessage &state)
{
    m_state = state;

    m_label->hide();

    m_camList->show();
    m_camNumber->show();
    m_stateWorkingList->show();

    m_stateWorkingList->clear();

    for (int i = 0; i < state.sectorCount; i++) {
        m_stateWorkingList->addState(state.cdoState[i], i);
    }
}

void StateWorkingWidget::initUI()
{
    m_exit->move(25, 15);
    m_exit->setText(" Назад ");
    m_exit->setStyleSheet("background-color: #CEA898;"
                          "font-size: 20px;");

    //--------------------------------------------------------------

    m_label->setStyleSheet("background-color: transparent;"
                           "font-size: 24px;"
                           "text-align: center;");
    m_label->setText("Ожидание получения данных\n"
                     "о текущем состоянии АС");

    m_label->setAlignment(Qt::AlignCenter);

    //--------------------------------------------------------------

    m_headerLabel->setStyleSheet("background-color: transparent;"
                                 "font-size: 26px;"
                                 "font-weight: 600;");
    m_headerLabel->setText("Информация о текущем состоянии АС");

    m_headerLabel->move(rect().left() + 120, 15);
}

void StateWorkingWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    painter.fillRect(rect(), QColor("#C3D7E4"));

    QPen pen = QPen();
    pen.setWidth(2);
    pen.setColor("black");
    painter.setPen(pen);

    QLine line = QLine(0, 60, rect().right(), 60);
    painter.drawLine(line);

    pen.setColor("black");
    painter.setPen(pen);

    QFont font;
    font.setPixelSize(18);
    painter.setFont(font);

    painter.drawText(50, 85, "Состояние работоспособности АС");
    painter.drawText(20, 235, "Информация о текущем состоянии СЧ АС");

    font.setPixelSize(16);
    painter.setFont(font);

    painter.drawText(70, 120, QString("Состояние ЭВМ"));
    painter.drawText(70, 140, QString("Состояние АС в целом"));
    painter.drawText(70, 160, QString("Состояние коммутатора 1"));
    painter.drawText(70, 180, QString("Состояние коммутатора 2"));
    painter.drawText(70, 200, QString("Состояние стандарта частоты"));


    pen.setColor(colorOnState(m_state.computerState));
    painter.setPen(pen);
    painter.drawText(300, 120, "🟢");

    pen.setColor(colorOnState(m_state.acState));
    painter.setPen(pen);
    painter.drawText(300, 140, "🟢");

    pen.setColor(colorOnState(m_state.switch1State));
    painter.setPen(pen);
    painter.drawText(300, 160, "🟢");

    pen.setColor(colorOnState(m_state.switch2State));
    painter.setPen(pen);
    painter.drawText(300, 180, "🟢");

    pen.setColor(colorOnState(m_state.frequencyState));
    painter.setPen(pen);
    painter.drawText(300, 200, "🟢");

    pen.setColor("black");
    painter.setPen(pen);

    drawStates(SwitchComputer, &painter, QPoint(320, 119), m_state.computerState);
    drawStates(AC, &painter, QPoint(320, 139), m_state.acState);
    drawStates(SwitchComputer, &painter, QPoint(320, 159), m_state.switch1State);
    drawStates(SwitchComputer, &painter, QPoint(320, 179), m_state.switch2State);
    drawStates(Frequency, &painter, QPoint(320, 199), m_state.frequencyState);

    pen.setColor("#708086");
    painter.setPen(pen);

    painter.drawLine(50, 95, 340, 95);

    painter.fillRect(m_label->rect(), m_label->isVisible() ? "#C3D7E4"
                                                           : "transparent");
}

QColor StateWorkingWidget::colorOnState(int state)
{
    switch(state) {
        case 0: return "green";
        case 1:
        case 2:
        case 3:
        case 4: return "yellow";
        default: return "red";
    }
}

void StateWorkingWidget::drawStates(StateRole state, QPainter *painter, QPoint point, qint32 stateInfo)
{
    QString result;

    if (stateInfo == 0) {
        result = "в норме";
        painter->drawText(point, result);
    }

    switch (state) {
        case AC:
            if (stateInfo == 1) {
                result = "имеются самовосстанавливающиеся отказы (сбои)";

            } else if (stateInfo == 2) {
                result = "имеются некритичные отказы";

            } else if (stateInfo == 3) {
                result = "имеются критичные отказы";

            } else if (stateInfo == 255) {
                result = "полный отказ";
            }

            painter->drawText(point, result);

            break;


        case Frequency:
            if (stateInfo == 1) {
                result = "отсутствует привязка во времени к ГНСС";

            } else if (stateInfo == 2) {
                result = "отсутствует сигнал 1Гц";

            } else if (stateInfo == 3) {
                result = "отсутствует сигнал 10МГц";

            } else if (stateInfo == 4) {
                result = "отказ генератора";

            } else if (stateInfo == 255) {
                result = "недоступен";
            }

            painter->drawText(point, result);

            break;


        case SwitchComputer:
            if (stateInfo == 1) {
                result = "имеются проблемы";
            } else if (stateInfo == 255) {
                result = "недоступен";
            }

            painter->drawText(point, result);

            break;

        default:
            break;
    }
}
