#ifndef GEOMETRYTOGEOJSON_H
#define GEOMETRYTOGEOJSON_H

#include <QObject>
#include <QJsonObject>
#include <geos/geom/Geometry.h>

using namespace geos::geom;

class GeometryToGeoJson : public QObject
{
    Q_OBJECT
public:
    static QJsonObject format( Geometry * geom, QJsonObject properties = QJsonObject() );

private:
    static QJsonObject parseGeometry( Geometry * geom );
};

#endif // GEOMETRYTOGEOJSON_H
