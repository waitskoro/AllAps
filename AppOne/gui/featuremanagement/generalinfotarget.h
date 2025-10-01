#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>

namespace View {

class GeneralInfoTarget : public QWidget
{
    Q_OBJECT
public:
    explicit GeneralInfoTarget(QWidget *parent = nullptr);

    double freq() {
        return m_frequency->text().toDouble();
    }

    int polarization() {
        return m_polarization->currentIndex();
    }

    int spacecraftNumber() {
        return m_spacecraftNumber->text().toInt();
    }

    int channel() {
        return m_channelData->currentText().toInt();
    }

signals:

private:
    void init();
    void paintEvent(QPaintEvent *event);

    QLineEdit *m_frequency;
    QComboBox *m_channelData;
    QComboBox *m_polarization;
    QLineEdit *m_spacecraftNumber;
};

}
