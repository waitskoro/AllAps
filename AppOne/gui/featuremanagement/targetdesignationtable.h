#pragma once

#include <QObject>
#include <QLineEdit>
#include <QTableView>
#include <QPushButton>

namespace View {

class TargetDesignationModel;

class TargetDesignationTable : public QWidget
{
    Q_OBJECT
public:
    explicit TargetDesignationTable(QWidget *parent = nullptr);

    const std::vector<std::array<short, 2>>& coordinates() const;

private:
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
