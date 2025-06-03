#pragma once

#include <QWidget>
#include <QTableView>

namespace View {

class TargetDesignationModel;

class TargetsView : public QWidget
{
    Q_OBJECT
public:

    struct Target {
        qint32 channelData;
        qint32 ka;
        qint32 sector;
        qint32 physicalChannel;
        qint32 countTargets;
    };

    explicit TargetsView(Target, QWidget *parent = nullptr);

    void setTargets(std::vector<std::array<short, 2>> &);

private:
    void paintEvent(QPaintEvent *event);

    void drawCommonInfo(QPainter *painter,
                        QRect *rect,
                        QString &title,
                        QString &info) const;

    Target m_target;
    QTableView *m_tableView;
    TargetDesignationModel *m_model;
};

}
