#include "OsmImporter.h"

#include <QFile>
#include <geos/geom/CoordinateSequenceFactory.h>

#include "environment/Environment.h"

OsmImporter::OsmImporter(QString file_path) : QObject(){
    QFile file(file_path);
    if( !file.exists() ){
        qWarning() << "[OsmImporter::OsmImporter] File" << file_path << "not found" << endl;
        return;
    }

    this->file_path = file_path;
}

/**********************************************************************
 GETTERS
**********************************************************************/

/**
 * @brief OsmImporter::getOsmElements
 * The file is parsed everytime this method is called to create new geometries
 * IMPORTANT CALLER GETS OWNERSHIP OF GEOMETRIES AND HAS TO DELETE THEM IF NOT IN USE
 * @return
 */
QList<OsmElement> OsmImporter::getOsmElements(bool nodes, bool ways, bool relations){

    QList<OsmElement> elements;

    // QMAP<ID , ELEMENT>
    QMap<QString , OsmElement> parsed_nodes;
    QMap<QString , OsmElement> parsed_ways;
    QMap<QString , OsmElement> parsed_relations;

    // Cant ensure they will be in order, so repeat the loop 3 times
    QFile file( this->file_path );
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){

        // Nodes
        QXmlStreamReader* xml_nodes = new QXmlStreamReader(&file);

        while( !xml_nodes->atEnd() ){
            xml_nodes->readNextStartElement();
            if ( xml_nodes->name() == "node" ){
                OsmElement node = this->processNode( xml_nodes );
                parsed_nodes.insert( node.id , node );
            }
        }
        if ( xml_nodes->hasError() ) {
            qWarning() << "[OsmImporter::OsmImporter] Error parsing OSM XML for nodes" << xml_nodes->errorString();
        }
        delete xml_nodes;
        file.close();
    }


    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        // Ways
        QXmlStreamReader* xml_ways = new QXmlStreamReader(&file);

        while( !xml_ways->atEnd() ){
            xml_ways->readNextStartElement();
            if ( xml_ways->name() == "way" ){
                OsmElement way = this->processWay( xml_ways , parsed_nodes );
                parsed_ways.insert( way.id , way );
            }
        }
        if ( xml_ways->hasError() ) {
            qWarning() << "[OsmImporter::OsmImporter] Error parsing OSM XML for ways" << xml_ways->errorString();
        }
        delete xml_ways;
        file.close();
    }


    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        // Relations
        QXmlStreamReader* xml_rels = new QXmlStreamReader(&file);

        while( !xml_rels->atEnd() ){
            xml_rels->readNextStartElement();
            if ( xml_rels->name() == "relation" ){
                //OsmElement relation = this->processRelation( parsed_nodes , parsed_ways );
                //parsed_relations.insert( relation.id , relation );
            }
        }
        if ( xml_rels->hasError() ) {
            qWarning() << "[OsmImporter::OsmImporter] Error parsing OSM XML for relations" << xml_rels->errorString();
        }
        delete xml_rels;
        file.close();
    }


    foreach (OsmElement e , parsed_nodes.values()) {
        if( nodes ){
            elements.append( e );
        } else {
            delete e.geometry;
        }
    }
    foreach (OsmElement e , parsed_ways.values()) {
        if( ways ){
            elements.append( e );
        } else {
            delete e.geometry;
        }
    }
    foreach (OsmElement e , parsed_relations.values()) {
        if( relations ){
            elements.append( e );
        } else {
            delete e.geometry;
        }
    }
    return elements;
}


OsmElement OsmImporter::processNode( QXmlStreamReader* xml ){
    OsmElement node;
    if ( !xml->isStartElement() || xml->name() != "node"){
        return node;
    }

    node.id = xml->attributes().value("id").toString();
    node.type = "node";
    double x = xml->attributes().value("lon").toDouble( 0 );
    double y = xml->attributes().value("lat").toDouble( 0 );
    double z = xml->attributes().value("ele").toDouble( 0 );

    while ( xml->readNextStartElement() ) {
        if ( xml->name() == "tag"){
            node.tags.insert( xml->attributes().value("k").toString() , xml->attributes().value("v").toString() );
        }
        xml->skipCurrentElement();
    }

    node.geometry = Environment::getInstance()->getGeometryFactory()->createPoint( Coordinate(x , y , z) );
    return node;
}

OsmElement OsmImporter::processWay( QXmlStreamReader* xml , QMap<QString , OsmElement> nodes ){
    OsmElement way;
    if ( !xml->isStartElement() || xml->name() != "way"){
        return way;
    }

    way.id = xml->attributes().value("id").toString();
    way.type = "way";
    CoordinateSequence* seq = Environment::getInstance()->getGeometryFactory()->getCoordinateSequenceFactory()->create();

    while ( xml->readNextStartElement() ) {
        if ( xml->name() == "tag"){
            way.tags.insert( xml->attributes().value("k").toString() , xml->attributes().value("v").toString() );
        }
        if ( xml->name() == "nd"){
            QString ref = xml->attributes().value("ref").toString();
            if( nodes.keys().contains( ref ) ){
                OsmElement node = nodes.value( ref );
                seq->add( Coordinate( node.geometry->getCoordinate()->x , node.geometry->getCoordinate()->y , node.geometry->getCoordinate()->z ) );
            }
        }
        xml->skipCurrentElement();
    }

    if ( seq->isEmpty() || seq->size() == 1 ){
        way.geometry = Environment::getInstance()->getGeometryFactory()->createEmptyGeometry();
    } else if( seq->getAt(0) != seq->getAt( seq->size()-1 ) ){
        way.geometry = Environment::getInstance()->getGeometryFactory()->createLineString( seq );
    } else {
        LinearRing* ring = Environment::getInstance()->getGeometryFactory()->createLinearRing( seq );
        std::vector<Geometry*> *holes;
        way.geometry = Environment::getInstance()->getGeometryFactory()->createPolygon( ring , holes );
    }

    return way;
}

OsmElement OsmImporter::processRelation( QXmlStreamReader* xml , QMap<QString, OsmElement> nodes, QMap<QString, OsmElement> ways){
    OsmElement relation;
    if ( !xml->isStartElement() || xml->name() != "relation"){
        return relation;
    }

    relation.id = xml->attributes().value("id").toString();
    relation.type = "relation";

    while ( xml->readNextStartElement() ) {
        if ( xml->name() == "tag"){
            relation.tags.insert( xml->attributes().value("k").toString() , xml->attributes().value("v").toString() );
        }
        if ( xml->name() == "nd"){

        }
        xml->skipCurrentElement();
    }

    return relation;
}
