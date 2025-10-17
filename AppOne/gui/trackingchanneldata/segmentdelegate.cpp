#include "segmentdelegate.h"

#include "segmentslist.h"
#include "src/contants.h"
#include "targetsview.h"
#include "src/common.h"

#include <QEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QPushButton>

using namespace View;

SegmentDelegate::SegmentDelegate(QWidget *parent)
    : QStyledItemDelegate(parent)
    , m_values(new QVector<TargetDesignation>())
{}

QSize SegmentDelegate::sizeHint(const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(280, 120);
}

void SegmentDelegate::paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const
{
    painter->save();

    painter->fillRect(option.rect, "#BDE5ED");

    painter->fillRect(QRect(option.rect.left(), option.rect.top(),
                            option.rect.width(), 30), "#3C388D");

    QFont font;
    font.setPixelSize(14);
    painter->setFont(font);

    QPen pen;
    pen.setColor("white");
    painter->setPen(pen);

    auto endTime = index.data(SegmentsList::EndTime);
    auto startTime = index.data(SegmentsList::StartTime);
    auto targetCount = index.data(SegmentsList::TargetCount);
    auto sectorNumber = index.data(SegmentsList::SectorNumber);
    auto centerFrequency = index.data(SegmentsList::CenterFrequency);
    auto spacecraftNumber = index.data(SegmentsList::SpacecraftNumber);
    auto physicalChannelNumber = index.data(SegmentsList::PhysicalChannelNumber);
    auto polarizationDirection = index.data(SegmentsList::PolarizationDirection);

    painter->drawText(option.rect.left() + 30, option.rect.top() + 20, "№ КА: " + spacecraftNumber.toString());
    painter->drawText(option.rect.left() + 110, option.rect.top() + 20, "№ физ. канала: " + physicalChannelNumber.toString());
    painter->drawText(option.rect.left() + 270, option.rect.top() + 20, "№ сектора: " + sectorNumber.toString());

    pen.setColor("black");
    painter->setPen(pen);
    painter->setFont(font);

    QDateTime startDate = fromDoubleToDate(startTime.toDouble());
    QDateTime endDate = fromDoubleToDate(endTime.toDouble());

    auto startTimeS = startDate.toString("hh:mm:ss");
    auto endTimeS =  endDate.toString("hh:mm:ss");

    auto freq = QString("Частота: %1 кГц").arg(centerFrequency.toString());

    QString polS;
    switch (polarizationDirection.toInt()) {
        case 1: polS = "правая круговая"; break;
        case 2: polS = "левая круговая"; break;
        case 3: polS = "вертикальная"; break;
        case 4: polS = "горизонтальная"; break;
        case 5: polS = "линейная +45°"; break;
        case 6: polS = "линейная –45°"; break;
        default: polS = "неизвестно"; break;
    }

    auto pol = QString("Поляризация: %1").arg(polS);

    painter->drawText(option.rect.left() + 10, option.rect.top() + 70, freq);
    painter->drawText(option.rect.left() + 170, option.rect.top() + 70, pol);
    painter->drawText(option.rect.left() + 70, option.rect.top() + 50,
                      "[" + startTimeS + " - " + endTimeS + "]");

    QRect buttonRect(option.rect.left() + 60, option.rect.top() + 80, 250, 30);

    QStyleOptionButton button;
    button.rect = buttonRect;
    button.text = "Просмотреть целеуказания";
    button.state = QStyle::State_Enabled;

    QPalette palette = button.palette;
    palette.setColor(QPalette::Button, "#78C4BB");
    button.palette = palette;

    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);

    painter->restore();
}

bool SegmentDelegate::editorEvent(QEvent *event,
                                  QAbstractItemModel *model,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease) {

        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QRect buttonRect(option.rect.left() + 60, option.rect.top() + 80, 250, 30);

        QVariant variant = index.data(SegmentsList::Targets);
        auto targetCount = index.data(SegmentsList::TargetCount);
        auto sectorNumber = index.data(SegmentsList::SectorNumber);
        auto spacecraftNumber = index.data(SegmentsList::SpacecraftNumber);
        auto physicalChannelNumber = index.data(SegmentsList::PhysicalChannelNumber);

        m_values->clear();

        int16_t** vectorData = variant.value<int16_t**>();

        for (int i = 0; i < targetCount.toInt(); i++) {
            TargetDesignation trg;
            trg.azimut = vectorData[i][0] / 10.0;
            trg.elev = vectorData[i][1] / 10.0;
            m_values->push_back(trg);
        }

        if (buttonRect.contains(mouseEvent->pos())) {
            TargetsView::Target target;
            target.ka = spacecraftNumber.toInt();
            target.sector = sectorNumber.toInt();
            target.countTargets = targetCount.toInt();
            target.physicalChannel = physicalChannelNumber.toInt();
            target.channelData = index.data(SegmentsList::ChannelNumber).toInt();

            TargetsView *view = new TargetsView(target);
            view->setWindowFlags(Qt::Dialog);
            view->setTargets(*m_values);
            view->show();
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
