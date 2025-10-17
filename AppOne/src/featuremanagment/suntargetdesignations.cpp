#include "suntargetdesignations.h"
#include <QDateTime>
#include <cmath>
#include <QtMath>
#include <QVector3D>

SunTargetDesignations::SunTargetDesignations(QObject *parent)
    : QObject(parent)
{}

void SunTargetDesignations::setCoords(Coordinates &coords)
{
    m_coords = coords;
}

QVector<Target> SunTargetDesignations::getTargets(const QDateTime start, const QDateTime end)
{
    QVector<Target> targets;

    auto seconds = end.toSecsSinceEpoch() - start.toSecsSinceEpoch();

    for (int i = 0; i < seconds; i++) {
        targets.push_back(convertToTargets(m_coords, start.addSecs(i)));
    }

    return targets;
}

static double dWR(double valOne, double valTwo) {
    return floor(valOne / valTwo);
}

double wrapTo360(double angle) {
    angle = fmod(angle, 360.0);
    if (angle < 0) {
        angle += 360.0;
    }
    return angle;
}

double wrapTo2Pi(double angle_rad) {
    angle_rad = fmod(angle_rad, 2 * M_PI);
    if (angle_rad < 0)
        angle_rad += 2 * M_PI;
    return angle_rad;
}

Target SunTargetDesignations::convertToTargets(Coordinates &coords, QDateTime date)
{
    double lat = coords.latitude();
    double lon = coords.longitude();

    const int CHP = 7;
    const double pres = coords.press();
    const double temperature = coords.temp();

    int year = date.date().year();
    int month = date.date().month();
    int day = date.date().day();
    int hour = date.time().hour();
    int minute = date.time().minute();
    int second = date.time().second();

    //------------------Вычисляется юлианская дата-----------------------------------------------------

    double a = dWR((14 - month), 12);
    double y = year + 4800 - a;
    double m = month + 12 * a - 3;

    double JDN = day + dWR((153 * m + 2), 5) + 365 * y + dWR(y, 4) - dWR(y, 100) + dWR(y, 400) - 32045;
    double JD = JDN + (hour - CHP - 12) / 24.0 + (minute / 1440.0) + (second / 86400.0);

    double n = JD - 2451545;

    double JC = n / 36525;
    double eO = 23.439291111111 - (46.815 / 3600) * JC - (0.0059 / 3600) * pow(JC, 2) + (0.00181 / 3600) * pow(JC, 3);
    double gO = 357.52910918 + 0.9856002585 * n;

    double lamO = 280.460 + 0.9856474 * n + 1.915 * sin(gO * M_PI / 180) + 0.02 * sin(gO * M_PI / 90);
    lamO = wrapTo360(lamO);

    double aRA = atan2(
        cos(eO * M_PI / 180) * sin(lamO * M_PI / 180),
        cos(lamO * M_PI / 180)
    );

    if (aRA < 0)
        aRA += 2 * M_PI;

    double numerator = std::sin(eO * M_PI / 180) * std::sin(lamO * M_PI / 180);
    double denominator = std::sqrt(
        std::pow(std::cos(lamO  * M_PI / 180), 2) +
        std::pow(std::cos(eO * M_PI / 180) * std::sin(lamO * M_PI / 180), 2)
    );

    double bDecl = std::atan2(numerator, denominator);

    double gmst = (4.89496121 + 6.300388098985 * n + 6.77 * pow(10, -6) * pow(JC,2)) * 180 / M_PI;
    gmst = wrapTo360(gmst);

    double lha = gmst + lon - aRA * 180 / M_PI;
    lha = wrapTo360(lha);

    double az = std::atan2(
        sin(lha * M_PI / 180) * cos(bDecl),
        cos(lha * M_PI / 180) * cos(bDecl) * sin(lat * M_PI / 180) - sin(bDecl) * cos(lat * M_PI / 180)
    );

    az = wrapTo2Pi(az + M_PI);

    double angle = asin(cos(lha * M_PI / 180) * cos(bDecl) * cos(lat * M_PI / 180)
                       + sin(bDecl) * sin(lat * M_PI / 180));

    if (angle * 180/M_PI > 1.0) {
        if (pres > 80 && pres < 110 && temperature > 200) {
            double term = angle + (3.1376e-3 / (angle + 8.92e-2));
            if (fabs(term) > 1e-10 && fabs(cos(term)) > 1e-10) {
                double refraction = (2.967e-4 / tan(term)) * (pres/101.325) * (283.15/temperature);
                angle += refraction;
            }
        }
    }

    angle = qBound(-M_PI/2, angle, M_PI/2);

    Target result;
    result.azimut = az * 180 / M_PI * 10;
    result.angle = angle * 180 / M_PI * 10;

    return result;
}


