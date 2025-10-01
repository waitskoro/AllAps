#pragma once

#include <QObject>

class Coordinates {

public:
    Coordinates() {}
    explicit Coordinates(double latitude, double longitude, double az, double beta, double press, double temp)
        : m_latitude(latitude)
        , m_longitude(longitude)
        , m_az(az)
        , m_beta(beta)
        , m_pres(press)
        , m_temperature(temp)
    {}

    double latitude() { return m_latitude; }
    double longitude() { return m_longitude; }
    double az() { return m_az; }
    double beta() { return m_beta; }
    double press() { return m_pres; }
    double temp() { return m_temperature; }

private:
    double m_latitude;
    double m_longitude;

    double m_az;
    double m_beta;

    double m_pres;
    double m_temperature;
};

struct Target {
    double azumit;
    double bean;
};

class SunTargetDesignations : public QObject
{
    Q_OBJECT
public:
    explicit SunTargetDesignations(QObject *parent = nullptr);

    void setCoords(Coordinates &coords);
    QVector<Target> getTargets(const QDateTime start, const QDateTime end);

signals:

private:
    Coordinates m_coords;

    Target convertToTargets(Coordinates &coords, QDateTime date);
    double calculateAzimuth(double lat, double lon, const QDateTime& date);
};
