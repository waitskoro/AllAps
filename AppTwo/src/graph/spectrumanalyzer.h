#pragma once

#include <QVector>
#include <QDebug>
#include <complex>
#include <cmath>

class SpectrumAnalyzer
{
public:
    using Complex = std::complex<double>;
    using Signal = QVector<Complex>;
    using Spectrum = QVector<Complex>;

    static Spectrum computeSpectrum(const Signal& signal) {
        size_t N = signal.size();
        Spectrum spectrum(N, 0);

        for (size_t k = 0; k < N; ++k) {
            for (size_t n = 0; n < N; ++n) {
                double angle = -2 * M_PI * k * n / N;
                spectrum[k] += signal[n] * Complex(std::cos(angle), std::sin(angle));
            }
        }
        return spectrum;
    }
};
