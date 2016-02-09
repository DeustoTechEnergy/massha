#include "GraphNode.h"

GraphNode::GraphNode(Coordinate coor) : Node( coor ){
}

GraphNode::~GraphNode(){
}

/**********************************************************************
 GETTERS
**********************************************************************/

QList<GraphEdge*> GraphNode::getDepartingEdges(){
    QList<GraphEdge*> edges;
    std::vector<DirectedEdge*> list = geos::planargraph::Node::getOutEdges()->getEdges();
    for(int i = 0; i < list.size() ; i++){
        edges.append( dynamic_cast<GraphEdge*>( list.at(i) ) );
    }
    return edges;
}

QString GraphNode::objectName(){
    return this->object_name;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void GraphNode::setObjectName(QString name){
    this->object_name = name;
}

void GraphNode::connect(GraphEdge *edge){
    if( edge->getStartNode() == this ){
        geos::planargraph::Node::addOutEdge( edge );
    }
}

void GraphNode::disconnect(GraphEdge *edge){
    if( edge->getStartNode() == this ){
        geos::planargraph::Node::getOutEdges()->remove( edge );
    }
}
