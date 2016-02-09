#include "GraphEdge.h"

GraphEdge::GraphEdge(GraphNode* start_node, GraphNode* end_node) : DirectedEdge( start_node , end_node , end_node->getCoordinate() , true ){
    this->cost = start_node->getCoordinate().distance( end_node->getCoordinate() );
    this->object_name = QString("%1 to %2").arg( start_node->objectName() ).arg( end_node->objectName() );
}

/**********************************************************************
 GETTERS
**********************************************************************/

GraphNode* GraphEdge::getStartNode(){
    return dynamic_cast<GraphNode*>( geos::planargraph::DirectedEdge::getFromNode() );
}

GraphNode* GraphEdge::getEndNode(){
    return dynamic_cast<GraphNode*>( geos::planargraph::DirectedEdge::getToNode() );
}

double GraphEdge::getCost(){
    return this->cost;
}

double GraphEdge::getLength(){
    return this->length;
}

QString GraphEdge::objectName(){
    return this->object_name;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void GraphEdge::setCost(double cost){
    this->cost = cost;
}

void GraphEdge::setLength(double length){
    this->length = length;
    this->cost = length;
}

void GraphEdge::setObjectName(QString name){
    this->object_name = name;
}

/**********************************************************************
 METHODS
**********************************************************************/
