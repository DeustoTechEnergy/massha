#ifndef PLACE_H
#define PLACE_H

#include <QObject>

#include "agents/Agent.h"

class Place : public Agent
{
    Q_OBJECT
public:
    Place(QString class_name = "Place");

    // GETTERS
    double getArea();

    // SETTERS
    void setArea(double area);

private:
    double area;

};

#endif // PLACE_H
