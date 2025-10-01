#include "dsp.h"
#include "complex.h"

#include "minfft/minfft.h"
#include "qdebug.h"

namespace {
int SIZE = 1024;
}

Dsp::Dsp(QObject *parent)
    : QObject(parent)
{}

bool Dsp::input(const ComplexVector &in, double period)
{
    double fSamp = 1.0/period;

    makeVector(0, period, in.size(), m_time);
    makeVector(fSamp * -0.5, fSamp * (1.0 / SIZE), SIZE, m_freq);

    extractIQ(in, m_i, m_q);

    ComplexVector windowed;
    ComplexVector tempBuffer;

    imposeWindowFunc(in, tempBuffer);
    dftTransform(tempBuffer, tempBuffer);
    dftAmpDist(tempBuffer, m_amp);
    ampDistToDb(m_amp, m_ampDb);

    double power = 0;

    m_ampDbAvg.clear();
    m_ampDbSumm.clear();
    m_ampDbAvg = m_ampDb;
    m_ampDbSumm = m_ampDb;

    for (int i = 0; i < m_amp.size(); i++)
        power += m_amp[i] * m_amp[i];

    m_power = power / m_amp.size();

    return true;
}

void Dsp::extractIQ(const ComplexVector & in, QVector<double> & outI, QVector<double> & outQ)
{
    outI.resize(in.size());
    outQ.resize(in.size());

    for(int i = 0; i < in.size(); i++){
        outI[i] = in[i].real();
        outQ[i] = in[i].imag();
    }
}

void Dsp::makeVector(double start, double step, int size, QVector<double> &out)
{
    if (size == 0)
        return;

    out.resize(size);
    out[0] = start;

    for (int i = 1; i < size; i++) {
        out[i] = out[i - 1] + step;
    }
}

void Dsp::add(const QVector<double> &in, QVector<double> &out)
{
    if (in.size() != out.size())
        out.resize(in.size());

    for (int i = 0; i < in.size(); i++)
        out[i] = in[i] + out[i];
}

void Dsp::div(const QVector<double> &in, QVector<double> &out, int div)
{
    if (in.size() != out.size())
        out.resize(in.size());

    for (int i = 0; i < in.size(); i++)
        out[i] = in[i] / div;
}

void Dsp::dftTransform(const ComplexVector &in, ComplexVector &out)
{
    const int size = 1024;

    out.resize(size);

    auto x = new minfft_cmpl[size];
    auto y = new minfft_cmpl[size];
    minfft_aux *a;
    a = minfft_mkaux_dft_1d(size);

    for (int i = 0; i <size; i++) {
        x[i][0] = in[i].real();
        x[i][1] = in[i].imag();
    }

    minfft_dft(x, y, a);

    for (int i = 0, iy = size/2; i < size; i++, iy++) {
        if (iy == size)
            iy = 0;
        out[i].real(y[iy][0]);
        out[i].imag(y[iy][1]);
    }

    minfft_free_aux(a);
    delete [] x;
    delete [] y;
}

void Dsp::ampDistToDb(const QVector<double> &in, QVector<double> &out)
{
    out.resize(in.size());

    for (int i = 0; i < in.size(); i++) {
        out[i] = 20 * log10(std::abs(in[i]));
    }
}

void Dsp::dftAmpDist(const QVector<Dsp::Complex> &in, QVector<double> &out)
{
    out.resize(in.size());

    for (int i = 0; i < in.size(); i++) {
        out[i] = std::abs(in[i]);
    }
}

void Dsp::imposeWindowFunc(const Dsp::ComplexVector &in, Dsp::ComplexVector &out)
{
    if (m_window.size() != in.size()) {
        out = in;
        return;
    }

    out.resize(in.size());

    for (int i = 0; i < m_window.size(); ++i) {
        out[i].imag(in[i].imag());
        out[i].real(in[i].real());
    }
}
