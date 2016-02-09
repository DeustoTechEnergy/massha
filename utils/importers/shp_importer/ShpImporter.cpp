#include "ShpImporter.h"
#include <QDebug>
#include <QDir>

#include "environment/Environment.h"

#include "geos/geom/GeometryFactory.h"
#include "geos/geom/CoordinateSequence.h"
#include "geos/geom/Point.h"
#include "geos/geom/LineString.h"
#include "geos/geom/LinearRing.h"
#include "geos/geom/Polygon.h"
#include "geos/geom/CoordinateArraySequenceFactory.h"


/**
 * @brief ShpToGeometry::open Give the path and file name WITHOUT EXTENSION of the SHP file that wants to be openned.
 * All SHP, SHX, DBF must have the same name.
 * For example if the SHP, SHX, DBF are called territory.shp, territory.shx, territory.dbf pass as parameter
 * ("/user/home/folder/" , "territory")
 * @param filepath
 * @return
 */
ShpImporter::ShpImporter(QString path , QString filename) : QObject(), shp_handle(0), dbf_handle(0), shape_count(0), shape_type(0){

    QDir dir(path);
    if( !dir.exists() ){
        return;
    }

    shp_handle = SHPOpen( dir.absoluteFilePath(filename).toStdString().c_str(), "rb");
    dbf_handle = DBFOpen( dir.absoluteFilePath(filename).toStdString().c_str(), "rb" );

    if ( !shp_handle && !dbf_handle ) {
        return;
    }
    else {
        SHPGetInfo(shp_handle, &shape_count, &shape_type, 0, 0);
    }
    return;
}

ShpImporter::~ShpImporter(){
    if ( this->shp_handle ) {
        SHPClose(shp_handle);
    }
    if ( this->dbf_handle ){
        DBFClose(dbf_handle);
    }
}

/**********************************************************************
 METHODS
**********************************************************************/

int ShpImporter::getShapeCount(){
    return shape_count;
}

Geometry* ShpImporter::getShape(int index , QJsonObject &properties)
{
    if ( !shp_handle ) {
        return 0;
    }

    SHPObject *obj = SHPReadObject( shp_handle, index );

    // Properties
    properties.insert("shape-id" , obj->nShapeId);
    if( dbf_handle ){
        for(int p = 0; p < DBFGetFieldCount( dbf_handle ); p++){

            char field_name[12];
            switch ( DBFGetFieldInfo( dbf_handle , p , field_name, 0, 0) ) {
            case FTString:
                properties.insert( QString( field_name ) , DBFReadStringAttribute( dbf_handle , obj->nShapeId , p ) );
                break;
            case FTInteger:
                properties.insert( QString( field_name ) , DBFReadIntegerAttribute( dbf_handle , obj->nShapeId , p ) );
                break;
            case FTDouble:
                properties.insert( QString( field_name ) , DBFReadDoubleAttribute( dbf_handle , obj->nShapeId , p ) );
                break;
            case FTLogical:
                properties.insert( QString( field_name ) , DBFReadLogicalAttribute( dbf_handle , obj->nShapeId , p ) );
                break;
            default:
                break;
            }
        }
    }

    // Geometry
    Geometry* geom = this->parseShape( obj );
    SHPDestroyObject( obj );
    return geom;
}


Geometry* ShpImporter::parseShape(SHPObject *obj){
    GeometryFactory* factory = Environment::getInstance()->getGeometryFactory();
    Geometry* dummy = factory->createEmptyGeometry();
    QList<CoordinateSequence*> coordinate_seq;

    for(int part = 0; part < obj->nParts; part++){

        CoordinateSequence* coordinates = dummy->getCoordinates();
        int part_vertices = 0;

        if (part == obj->nParts - 1) {
            // Last (or unique) part
            part_vertices = obj->nVertices - obj->panPartStart[part];
        } else {
            part_vertices = obj->panPartStart[part + 1] - obj->panPartStart[part];
        }

        for(int vertex = obj->panPartStart[part]; vertex < obj->panPartStart[part] + part_vertices; vertex++){
            Coordinate coor(obj->padfX[vertex] , obj->padfY[vertex] , obj->padfZ[vertex]);
            coordinates->add( coor );
        }
        coordinate_seq.append( coordinates );
    }
    delete dummy;

    switch( obj->nSHPType ){
    case SHPT_POINT:
    case SHPT_POINTZ:
    case SHPT_POINTM:
        return factory->createPoint( coordinate_seq.at(1) );

    case SHPT_ARC:
    case SHPT_ARCZ:
    case SHPT_ARCM:
        return factory->createLineString( coordinate_seq.at(1) );

    case SHPT_POLYGON:
    case SHPT_POLYGONZ:
    case SHPT_POLYGONM: {

        LinearRing* shell = 0;
        std::vector<Geometry*> holes;

        foreach(CoordinateSequence* coors , coordinate_seq){

            if( !coors->front().equals( coors->back() ) ){
                coors->add( coors->front() );
            }

            if( !shell ){
                shell = factory->createLinearRing( coors );
            } else {

                LinearRing* r = factory->createLinearRing( coors );
                holes.push_back( r );
            }
        }
        return factory->createPolygon( *shell , holes );
    }

    case SHPT_MULTIPOINT:
    case SHPT_MULTIPOINTZ:
    case SHPT_MULTIPOINTM:

        break;

    default:
        break;
    }

    return 0;
}
