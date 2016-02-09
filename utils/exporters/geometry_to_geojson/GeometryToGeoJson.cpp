#include <QDebug>
#include <string.h>
#include <QJsonArray>

#include <geos/geom/Point.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/MultiPoint.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/MultiPolygon.h>

#include "GeometryToGeoJson.h"

QJsonObject GeometryToGeoJson::format(Geometry *geometry, QJsonObject properties){
    QJsonObject geojson;

    geojson.insert("type", "Feature");

    // Properties
    if( properties.length() ){
        geojson.insert("properties", properties);
    } else {
        QJsonObject empty;
        geojson.insert("properties", empty);
    }

    // Geometry
    if( geometry && geometry->isValid() ){
        geojson.insert("geometry", GeometryToGeoJson::parseGeometry( geometry ) );
    } else {
        QJsonValue empty;
        geojson.insert("geometry" , empty);
    }

    return geojson;
}

QJsonObject GeometryToGeoJson::parseGeometry(Geometry *geom){

    // Geometry
    QJsonObject geometry;

    try {

        // POINT
        if( geom->getGeometryTypeId() == geos::geom::GEOS_POINT ){
            Point* point = dynamic_cast<Point*>(geom);
            geometry.insert("type", "Point");

            QJsonArray coordinates;
            coordinates.append( point->getCoordinate()->x );
            coordinates.append( point->getCoordinate()->y );
            coordinates.append( point->getCoordinate()->z );
            geometry.insert("coordinates", coordinates);

        // LINESTRING
        } else if( geom->getGeometryTypeId() == geos::geom::GEOS_LINESTRING ){
            LineString* line = dynamic_cast<LineString*>(geom);
            geometry.insert("type", "LineString");

            QJsonArray lineCoordinates;

            for(unsigned int i = 0; i < line->getCoordinates()->size(); i++){
                QJsonArray coordinatePair;
                coordinatePair.append(line->getCoordinates()->getAt(i).x);
                coordinatePair.append(line->getCoordinates()->getAt(i).y);
                coordinatePair.append(line->getCoordinates()->getAt(i).z);
                lineCoordinates.append(coordinatePair);
            }
            geometry.insert("coordinates", lineCoordinates);

        // POLYGON
        } else if( geom->getGeometryTypeId() == geos::geom::GEOS_POLYGON ) {
            Polygon* polygon = dynamic_cast<Polygon*>(geom);
            geometry.insert("type", "Polygon");

            QJsonArray rings;

            QJsonArray ringCoordinates;
            for(unsigned int i = 0; i < polygon->getExteriorRing()->getCoordinates()->size(); i++){
                QJsonArray coordinatePair;
                coordinatePair.append( polygon->getExteriorRing()->getCoordinates()->getAt( i ).x );
                coordinatePair.append( polygon->getExteriorRing()->getCoordinates()->getAt( i ).y );
                coordinatePair.append( polygon->getExteriorRing()->getCoordinates()->getAt( i ).z );
                ringCoordinates.append(coordinatePair);
            }
            rings.append(ringCoordinates);

            for(unsigned int i = 0; i < polygon->getNumInteriorRing(); i++){
                QJsonArray ringCoordinates;
                for(unsigned int j = 0; j < polygon->getInteriorRingN( i )->getCoordinates()->size(); j++){
                    QJsonArray coordinatePair;
                    coordinatePair.append( polygon->getInteriorRingN( i )->getCoordinates()->getAt( j ).x );
                    coordinatePair.append( polygon->getInteriorRingN( i )->getCoordinates()->getAt( j ).y );
                    coordinatePair.append( polygon->getInteriorRingN( i )->getCoordinates()->getAt( j ).z );
                    ringCoordinates.append(coordinatePair);
                }
                rings.append(ringCoordinates);
            }

            geometry.insert("coordinates", rings);

        // MULTIPOINT
        } else if( geom->getGeometryTypeId() == geos::geom::GEOS_MULTIPOINT ||
                   geom->getGeometryTypeId() == geos::geom::GEOS_MULTILINESTRING ||
                   geom->getGeometryTypeId() == geos::geom::GEOS_MULTIPOLYGON ) {

            Geometry* multi = geom;

            if( geom->getGeometryTypeId() == geos::geom::GEOS_MULTIPOINT ){ geometry.insert("type", "MultiPoint"); }
            if( geom->getGeometryTypeId() == geos::geom::GEOS_MULTILINESTRING ){ geometry.insert("type", "MultiLineString"); }
            if( geom->getGeometryTypeId() == geos::geom::GEOS_MULTIPOLYGON ){ geometry.insert("type", "MultiPolygon"); }

            QJsonArray geomCoordinates;

            for(unsigned int i = 0; i < multi->getNumGeometries(); i++){
                Geometry* g = multi->getGeometryN(i)->clone();
                geomCoordinates.append( GeometryToGeoJson::format(g).value("geometry").toObject().value("coordinates") );
                delete g;
            }
            geometry.insert("coordinates", geomCoordinates);

        // GEOMETRYCOLLECTION
        } else if( geom->getGeometryTypeId() == geos::geom::GEOS_GEOMETRYCOLLECTION ) {

            geometry.insert("type" , "GeometryCollection");
            QJsonArray geometries;

            for(unsigned int i = 0; i < geom->getNumGeometries(); i++){
                Geometry* clone = geom->getGeometryN( i )->clone();
                geometries.append( GeometryToGeoJson::parseGeometry( clone ) );
                delete clone;
            }
            geometry.insert( "geometries" , geometries);

        } else {
            qWarning() << "[GeometryToGeoJson::parseGeometry] Geometry type" << QString::fromStdString( geom->getGeometryType() ) << "unknown";
        }

    } catch ( ... ){
        qWarning() << "[GeometryToGeoJson::parseGeometry] Geometry type parsing crashed";
    }

    return geometry;
}
