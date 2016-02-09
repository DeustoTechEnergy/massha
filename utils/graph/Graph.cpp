#include "Graph.h"

#include "environment/Environment.h"
#include "geos/geom/CoordinateArraySequenceFactory.h"

Graph::Graph() : PlanarGraph(){
}

/**********************************************************************
 GETTERS
**********************************************************************/

GraphEdge* Graph::findEdge(GraphNode *from, GraphNode *to){
    if( !this->findNode( from->getCoordinate() ) ){
        return 0;
    }
    if( !this->findNode( to->getCoordinate() ) ){
        return 0;
    }
    for(unsigned int i = 0; i < this->dirEdges.size(); i++){
        GraphEdge* edge = dynamic_cast<GraphEdge*>( this->dirEdges.at(i) );
        if( edge->getStartNode() == from && edge->getEndNode() == to ){
            return edge;
        }
    }
    return 0;
}

GraphNode* Graph::findNode(Coordinate coor){
    geos::planargraph::Node* n = geos::planargraph::PlanarGraph::findNode( coor );
    if( n ){
        return dynamic_cast<GraphNode*>( n );
    }
    return 0;
}

QList<GraphEdge*> Graph::getEdges(){
    QList<GraphEdge*> edges;
    for(unsigned int i = 0; i < this->dirEdges.size(); i++){
        edges.append( dynamic_cast<GraphEdge*>( this->dirEdges.at(i) ) );
    }
    return edges;
}

QList<GraphNode*> Graph::getNodes(){
    QList<GraphNode*> nodes;
    std::vector<geos::planargraph::Node*> ns;
    geos::planargraph::PlanarGraph::getNodes( ns );
    for(int i = 0; i < ns.size(); i++){
        nodes.append( dynamic_cast<GraphNode*>( ns.at(i) ) );
    }
    return nodes;
}

int Graph::countNodes(){
    return this->getNodes().size();
}

int Graph::countEdges(){
    return this->dirEdges.size();
}

MultiLineString* Graph::toMultiLineString(){
    std::vector<Geometry*> *lines = new std::vector<Geometry*>();
    foreach( GraphEdge* e , this->getEdges() ){
        std::size_t s = 0;
        CoordinateSequence* seq = Environment::getInstance()->getGeometryFactory()->getCoordinateSequenceFactory()->create( s , s );
        seq->add( e->getStartNode()->getCoordinate() );
        seq->add( e->getEndNode()->getCoordinate() );
        Geometry* line = Environment::getInstance()->getGeometryFactory()->createLineString( seq );
        lines->push_back( line );
    }
    return Environment::getInstance()->getGeometryFactory()->createMultiLineString( lines );
}

/**********************************************************************
 SETTERS0
**********************************************************************/

void Graph::addEdge(GraphEdge *edge){
    geos::planargraph::PlanarGraph::add( edge );
}

void Graph::addNode(GraphNode *node){
    geos::planargraph::PlanarGraph::add( node );
}

void Graph::removeEdge(GraphEdge *edge){
    geos::planargraph::PlanarGraph::remove( edge );
}

void Graph::removeNode(GraphNode *node){
    geos::planargraph::PlanarGraph::remove( node );
}
