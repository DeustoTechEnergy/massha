#include "GeoJsonToGeometry.h"

#include "geos/geom/Point.h"
#include "geos/geom/LineString.h"
#include "geos/geom/LinearRing.h"
#include "geos/geom/CoordinateSequenceFactory.h"

#include "environment/Environment.h"

Geometry* GeoJsonToGeometry::format(QJsonObject geojson){
    if ( geojson.isEmpty() ) {
        return 0;
    }

    QJsonObject geometry = geojson;
    if( geojson.contains("geometry") ){
        geometry = geojson["geometry"].toObject();
    }

    // http://geojson.org/geojson-spec.html

    GeometryFactory* factory = Environment::getInstance()->getGeometryFactory();

    if( geometry["type"].toString().toLower() == "point" ){

        QJsonArray c = geometry["coordinates"].toArray();
        Coordinate coor(c[0].toDouble() , c[1].toDouble() , (c.size() > 2) ? c[2].toDouble() : 0);
        return factory->createPoint( coor );
    }

    if( geometry["type"].toString().toLower() == "linestring" ){

        QJsonArray l = geometry["coordinates"].toArray();
        CoordinateSequence* coordinates = factory->getCoordinateSequenceFactory()->create((size_t)0, (size_t)0);

        for(int i = 0; i < l.size(); i++){
            QJsonArray c = geometry["coordinates"].toArray().at(i).toArray();
            Coordinate coor(c[0].toDouble() , c[1].toDouble() , (c.size() > 2) ? c[2].toDouble() : 0);
            coordinates->add(coor);
        }
        return factory->createLineString(coordinates);
    }

    if( geometry["type"].toString().toLower() == "polygon" ){

        QJsonArray p = geometry["coordinates"].toArray();
        Geometry* dummy = factory->createEmptyGeometry();
        LinearRing* shell = 0;
        std::vector<Geometry*> holes;

        for(int i = 0; i < p.size(); i++){
            QJsonArray l = geometry["coordinates"].toArray().at(i).toArray();
            CoordinateSequence* coordinates = dummy->getCoordinates();
            for(int j = 0; j < l.size(); j++){
                QJsonArray c = geometry["coordinates"].toArray().at(i).toArray().at(j).toArray();
                Coordinate coor(c[0].toDouble() , c[1].toDouble() , (c.size() > 2) ? c[2].toDouble() : 0);
                coordinates->add(coor);
            }
            if( !shell ){
                shell = factory->createLinearRing( coordinates );
            } else {
                LinearRing* r = factory->createLinearRing( coordinates );
                holes.push_back( r );
            }
        }
        delete dummy;
        return factory->createPolygon( *shell , holes );
    }
    return 0;
}

