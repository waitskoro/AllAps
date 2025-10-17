#pragma once

#include <QObject>
#include <QLineEdit>
#include <QTableView>
#include <QPushButton>

struct TargetDesignation;

namespace View {

class TargetDesignationModel;

class ManualTabTargets : public QWidget
{
    Q_OBJECT
public:
    explicit ManualTabTargets(TargetDesignationModel *model, QWidget *parent = nullptr);

    int beam();
    int countCoordinates() const;
    const QVector<TargetDesignation> &coordinates() const;
private:
    void init();
    void paintEvent(QPaintEvent *event);

    QTableView *m_tableView;
    QLineEdit *m_currentAzimut;
    QLineEdit *m_currentBeanAzimut;

    QPushButton *m_addButton;
    QPushButton *m_removeButton;
    QPushButton *m_testButton;
    QPushButton *m_clearAll;

    TargetDesignationModel *m_model;

    void onTestCreating();
    void onAddCoordinates();
    void onRemoveCoordinates();
    void onClearAllCoordinates();
};

}
