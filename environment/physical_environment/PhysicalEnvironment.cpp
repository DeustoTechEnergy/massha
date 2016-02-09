#include <QDebug>
#include <QJsonObject>

#include <geos/geom/PrecisionModel.h>
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/Envelope.h>
#include <geos/geom/Point.h>
#include <geos/io/WKTReader.h>

#include "PhysicalEnvironment.h"

#include "skills/random_generators/UniformDistributionSkill.h"
#include "utils/exporters/geometry_to_geojson/GeometryToGeoJson.h"

PhysicalEnvironment::PhysicalEnvironment(){
    // Create geometry factory
    PrecisionModel* precision_model = new PrecisionModel( PrecisionModel::FLOATING );
    this->geometry_factory = new GeometryFactory( precision_model, 4326 );

    // Create graph
    this->spatial_graph = new Graph();
}

PhysicalEnvironment::~PhysicalEnvironment(){
    delete this->geometry_factory;
    delete this->spatial_graph;
    qDeleteAll(this->spatial_index);
}

QJsonObject PhysicalEnvironment::physicalToJSON(){
    MultiLineString* geom = this->spatial_graph->toMultiLineString();
    QJsonObject json = GeometryToGeoJson::format( geom );
    delete geom;
    return json;
}

/**********************************************************************
 GETTERS
**********************************************************************/

GeometryFactory* PhysicalEnvironment::getGeometryFactory(){
    return this->geometry_factory;
}

geos::geom::Geometry* PhysicalEnvironment::getGeometryFromWKT(QString wkt){
    // Mutex, for avoiding concurrency
    QMutex mutex;
    try{
        QMutexLocker locker(&mutex);
        geos::io::WKTReader reader = geos::io::WKTReader( this->getGeometryFactory() );
        return reader.read( wkt.trimmed().toStdString() );
    } catch(...){}
    return 0;
}

QString PhysicalEnvironment::getWKTFromGeometry(geos::geom::Geometry* geometry){
    if ( geometry ){
        return QString::fromStdString( geometry->toString() );
    }
    return QString::fromStdString( this->getGeometryFactory()->createEmptyGeometry()->toString() );
}

Point* PhysicalEnvironment::getRandomPoint(geos::geom::Geometry* bounds , int seed){

    // If no bounds are given
    if( !bounds ){
        return 0;
    }

    // If the geometry given is a point
    if( bounds->getNumPoints() == 1 ){
        return bounds->getCentroid();
    }

    // Mutex, for avoiding concurrency
    QMutex mutex;
    QMutexLocker locker(&mutex);

    // Prepare for random points generation
    qsrand( seed );
    Point* point = 0;
    int max_try = 4;
    double left = bounds->getEnvelopeInternal()->getMinX();
    double right = bounds->getEnvelopeInternal()->getMaxX();
    double top = bounds->getEnvelopeInternal()->getMaxY();
    double bottom = bounds->getEnvelopeInternal()->getMinY();
    UniformDistributionSkill* skill = new UniformDistributionSkill();

    // Iterate untill getting a point inside
    do{
        if (point) { delete point; }      

        double x = left + (skill->uniformDistribution() * ( right - left ));
        double y = bottom + (skill->uniformDistribution() * ( top - bottom ));
        point = this->geometry_factory->createPoint( Coordinate(x , y) );
        max_try--;
    }  while( !bounds->contains( point ) && max_try > 0 );

    // Delete stuff
    delete skill;

    if( max_try == 0){
        delete point;
        return bounds->getInteriorPoint();
    }

    return point;
}

QList<Agent*> PhysicalEnvironment::orderByDistance(Agent* source, QList<Agent*> agents){
    QList<Agent*> ordered;
    if( !agents.size() ){
        return ordered;
    }

    while( agents.size() ){
        Agent* nearest = agents.at( 0 );

        foreach( Agent* agent , agents ){
            if( source->getGeometry()->distance( agent->getGeometry() ) < source->getGeometry()->distance( nearest->getGeometry() ) ){
                nearest = agent;
            }
        }
        ordered.append( nearest );
        agents.removeOne( nearest );
    }
    return ordered;
}

QList<Agent*> PhysicalEnvironment::getAgentsByGeometry(geos::geom::Geometry* geometry, QString class_name){
    QList<Agent*> agents;
    if( this->spatial_index.contains(class_name) ){
        std::vector<void*> vector;
        this->spatial_index[class_name]->query( geometry->getEnvelopeInternal() , vector);
        for(unsigned int i = 0 ; i < vector.size() ; i++){
            Agent* agent = ((Agent*) vector.at(i));
            if( geometry->intersects( agent->getGeometry() ) ){
                agents.append( agent );
            }
        }
    }
    return agents;
}

/**
 * @brief PhysicalEnvironment::getNearestAgents Given a list of geometries and the class_name of agents in the
 * environment, returns an ordered list of the nearest agent for each geometry.
 * @param geometry Geometry from where to start searching
 * @param class_name ClassName of the agents in the environment from which to get the nearest for geometry
 * @return
 */
Agent* PhysicalEnvironment::getNearestAgent(geos::geom::Geometry* geometry, QString class_name){
    Agent* found = 0;
    if( this->spatial_index.contains(class_name) ){
        std::vector<void*> vector;
        Envelope* env = this->spatial_index[class_name]->ensureExtent( geometry->getEnvelopeInternal() , 0 );
        this->spatial_index[class_name]->query( env , vector);
        delete env;
        if( vector.size() ){
            found = ( (Agent*) vector.at( qrand() % vector.size() ) );
            double found_distance = geometry->distance( found->getGeometry() );
            for(unsigned int i = 0 ; i < vector.size() ; i++){
                Agent* agent = ((Agent*) vector.at(i));
                if( agent ){
                    double d = geometry->distance( agent->getGeometry() );
                    if( d < found_distance ){
                        found = agent;
                        found_distance = d;
                    }
                }
            }
        }
    }
    return found;
}

/**
 * @brief PhysicalEnvironment::getNearestAgents Given a list of geometries and the class_name of agents in the
 * environment, returns an ordered list of the nearest agent for each geometry.
 * IMPORTANT, if an agent is not snapped it will return a 0 agent to preserve list size and order.
 * @param geometries List of geometries from where to start searching
 * @param class_name ClassName of the agents in the environment from which to get the nearest for each geometry
 * @return
 */
QList<Agent*> PhysicalEnvironment::getNearestAgents(QList<geos::geom::Geometry*> geometries, QString class_name){
    QList<Agent*> founds = QList<Agent*>();
    foreach(geos::geom::Geometry* geometry , geometries){
        founds.append( this->getNearestAgent( geometry , class_name ) );
    }
    return founds;
}

Agent* PhysicalEnvironment::getNearestAgent(geos::geom::Geometry* geometry, QList<Agent*> agents){
    QList<geos::geom::Geometry*> geometries;
    geometries.append( geometry );
    QList<Agent*> nearests = this->getNearestAgents( geometries , agents );
    if( !nearests.isEmpty() ){
        return nearests.at(0);
    }
    return 0;
}

/**
 * @brief PhysicalEnvironment::getNearestAgents Given a list of geometries and some agents to search, returns
 * an ordered list of the nearest agent for each geometry.
 * IMPORTANT, if an agent is not snapped it will return a 0 agent to preserve list size and order.
 * @param geometries List of geometries from where to start searching
 * @param agents List of agents from which to get the nearest for each geometry
 * @return
 */
QList<Agent*> PhysicalEnvironment::getNearestAgents(QList<geos::geom::Geometry*> geometries, QList<Agent*> agents){
    QList<Agent*> founds;
    Quadtree* index = new Quadtree();
    for(Agent* agent : agents){
        Envelope* env = index->ensureExtent( agent->getGeometry()->getEnvelopeInternal() , 0 );
        index->insert( env , agent );
        delete env;
    }
    foreach(geos::geom::Geometry* geometry , geometries){
        Agent* found = 0;
        std::vector<void*> vector;
        if( geometry ){
            index->query( geometry->getEnvelopeInternal() , vector);
            if( vector.size() ){
                found = ( (Agent*) vector.front() );
                for(unsigned int i = 0 ; i < vector.size() ; i++){
                    Agent* agent = ((Agent*) vector.at(i));
                    if( geometry->distance( agent->getGeometry() ) < geometry->distance( found->getGeometry() ) ){
                        found = agent;
                    }
                }
            }
        }
        founds.append( found );
    }

    delete index;
    return founds;
}

Agent* PhysicalEnvironment::getAgentByGeometry(geos::geom::Geometry *geometry, QString class_name){
    if( this->spatial_index.contains(class_name) ){

        std::vector<void*> vector;
        this->spatial_index[class_name]->query( geometry->getEnvelopeInternal() , vector);

        if( vector.size() ){
            for(unsigned int i = 0 ; i < vector.size() ; i++){
                Agent* agent = ( (Agent*) vector.at(i) );
                if( agent->getGeometry()->equals( geometry ) ){
                    return agent;
                }
            }
        }
    }
    return 0;
}

/**********************************************************************
 PHYSICAL GRAPH METHODS
**********************************************************************/

bool PhysicalEnvironment::arePhysicallyConnected(GraphNode *from, GraphNode *to){
    return this->spatial_graph->findEdge( from , to ) != 0;
}

GraphEdge* PhysicalEnvironment::physicalConnectNodes(GraphEdge* edge){
    this->spatial_graph->addEdge( edge );
    edge->getStartNode()->connect( edge );
    edge->getEndNode()->connect( edge );
    return edge;
}

GraphEdge* PhysicalEnvironment::physicalConnectNodes(GraphNode* from, GraphNode* to, bool bidirection){
    GraphEdge* edge = this->spatial_graph->findEdge( from , to );
    if( !edge ){
        edge = new GraphEdge( from , to );
        this->spatial_graph->addEdge( edge );
        from->connect( edge );
        to->connect( edge );
    }
    if( bidirection ){
        this->physicalConnectNodes( to , from , false );
    }
    return edge;
}

void PhysicalEnvironment::physycalDisconnectNodes(GraphNode* from, GraphNode* to){
    GraphEdge* edge = this->spatial_graph->findEdge( from , to );
    if( edge ){
        this->spatial_graph->removeEdge( edge );
        from->disconnect( edge );
        to->disconnect( edge );
    }
}

QMap<GraphEdge* , double> PhysicalEnvironment::getPhysicalGraph(){
    QMap<GraphEdge* , double> edges;
    foreach(GraphEdge* e , this->spatial_graph->getEdges()){
        edges.insert( e , e->getCost() );
    }
    return edges;
}

/**********************************************************************
 AGENT METHODS
**********************************************************************/

bool PhysicalEnvironment::addAgent(Agent *agent){

    {
        QMutexLocker locker(&this->mutex);
        // If it is the first agent of this class we receive, create the class in the spatial index
        if( !this->spatial_index.keys().contains( agent->getClassName() ) ){
            // Insert new quadtree with the agents class
            Quadtree* index = new Quadtree();
            this->spatial_index.insert(agent->getClassName() , index);
        }
    }

    // Add to spatial index
    if( agent->getGeometry() && agent->getGeometry()->getEnvelopeInternal() ){
        Envelope* env = this->spatial_index.value( agent->getClassName() )->ensureExtent( agent->getGeometry()->getEnvelopeInternal() , 0 );
        this->spatial_index.value( agent->getClassName() )->insert( env , agent);
        delete env;
    } else {
        qWarning() << QString("[PhysicalEnvironment::addAgent] Tried to add agent without geometry.") << endl;
    }

    // Add to spatial graph
    if( agent->getGeometry() && agent->getGeometry()->getEnvelopeInternal() ){
        GraphNode* node = this->spatial_graph->findNode( *agent->getGeometry()->getCoordinate() );
        if( !node ){
            node = new GraphNode( *agent->getGeometry()->getCoordinate() );
            this->spatial_graph->addNode( node );
        }
        agent->setGraphNode( node );
        node->setObjectName( agent->objectName() );
    } else {
        qWarning() << QString("[PhysicalEnvironment::addAgent] Tried to add agent without geometry.") << endl;
    }

    return true;
}

bool PhysicalEnvironment::removeAgent(Agent *agent){
    if( this->spatial_index.keys().contains( agent->getClassName() ) && agent->getGeometry() && agent->getGeometry()->getEnvelopeInternal() ){
        this->spatial_index.value( agent->getClassName() )->remove(agent->getGeometry()->getEnvelopeInternal() , agent);
    }
    return true;
}

geos::geom::Geometry* PhysicalEnvironment::updateAgentGeometry(Agent* agent, geos::geom::Geometry* old_geom, geos::geom::Geometry* new_geom){

    geos::geom::Geometry* geometry = new_geom->clone();

    if( old_geom ){ delete old_geom; }
    if( new_geom ){ delete new_geom; }

    GraphNode* node = this->spatial_graph->findNode( *geometry->getCoordinate() );
    if( !node ){
        node = new GraphNode( *geometry->getCoordinate() );
        this->spatial_graph->addNode( node );
    }
    agent->setGraphNode( node );
    node->setObjectName( agent->objectName() );

    return geometry;
}
