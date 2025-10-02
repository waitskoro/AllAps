#pragma once

#include <QObject>

class Coordinates {

public:
    Coordinates() {}
    explicit Coordinates(double latitude, double longitude)
        : m_latitude(latitude)
        , m_longitude(longitude)
    {}

    double latitude() { return m_latitude; }
    double longitude() { return m_longitude; }
    double press() { return m_pres; }
    double temp() { return m_temperature; }

private:
    double m_latitude;
    double m_longitude;

    double m_pres = 800;
    double m_temperature = 25;
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
