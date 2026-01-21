#pragma once

#include <QVector>
#include <QObject>

#include "complex"

class Dsp : public QObject
{
public:
    typedef std::complex<double> Complex;
    typedef QVector<Complex> ComplexVector;

    explicit Dsp(QObject *parent = nullptr);

    bool input(const ComplexVector & in, double period = 1./110.e3);

    void averagingCoe(int in) {
        m_averagingCoe = in;
        m_averagingIteration = AVG_DEFAULT;
        m_ampDbSumm.clear();
    }

    int averagingCoe() {
        return m_averagingCoe;
    }

    int averagingIteration() {
        return m_averagingIteration;
    }

    double powerFreqDb() {
        return 10*log10(this->m_power);
    }

    double powerFreq() {
        return this->m_power;
    }

    const QVector<double> & q() {
        return m_q;
    }

    const QVector<double> & i() {
        return m_i;
    }

    const QVector<double> & timeVector() {
        return m_time;
    }

    const QVector<double> & ampDist() {
        return m_amp;
    }

    const QVector<double> & phDist() {
        return m_ph;
    }

    const QVector<double> & freqVector() {
        return m_freq;
    }

    const QVector<double> & ampDistDb() {
        return m_ampDb;
    }

    const QVector<double> & ampDistDbAvg() {
        return m_ampDbAvg;
    }

    QVector<double> & window() {
        return m_window;
    }

    void makeVector(double start, double step, int size, QVector<double> & out);

private:
    void extractIQ(const ComplexVector & in, QVector<double> & outI, QVector<double> & outQ);

    void add(const QVector<double> & in, QVector<double> & out);
    void div(const QVector<double> & in, QVector<double> & out, int div = 2);

    void dftTransform(const ComplexVector & in, ComplexVector & out);
    void dftAmpDist(const ComplexVector & in, QVector<double> & out);
    void ampDistToDb(const QVector<double> & in, QVector<double> & out);
    void imposeWindowFunc(const ComplexVector & in, ComplexVector & out);

    const int AVG_DEFAULT = 1;

    QVector<double> m_q;
    QVector<double> m_i;
    QVector<double> m_ph;
    QVector<double> m_amp;
    QVector<double> m_time;
    QVector<double> m_freq;
    QVector<double> m_ampDb;
    QVector<double> m_ampDbAvg;
    QVector<double> m_ampDbSumm;

    double m_power = 0;
    int m_averagingCoe = 0;
    int m_averagingIteration = AVG_DEFAULT;

    QVector<double> m_window;
};
