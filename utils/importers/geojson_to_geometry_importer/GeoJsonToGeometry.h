#ifndef GEOJSONTOGEOMETRY_H
#define GEOJSONTOGEOMETRY_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <geos/geom/Geometry.h>

using namespace geos::geom;

class GeoJsonToGeometry : public QObject
{
    Q_OBJECT
public:
    static Geometry* format( QJsonObject geojson );
};

#endif // GEOJSONTOGEOMETRY_H
