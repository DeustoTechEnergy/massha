#include "Road.h"

#include "environment/Environment.h"

Road::Road(RoadConstants::road_types type, GraphNode* start_node, GraphNode* end_node, QString class_name) : Agent( class_name ) , GraphEdge( start_node , end_node ){
    this->setRoadType( type );
    this->setLength( this->getStartNode()->getCoordinate().distance( this->getEndNode()->getCoordinate() ) );
    this->setMaxSpeed( RoadConstants::roadTypeDefaultSpeed( this->getRoadType() ) );
    this->setCost( this->getLength() / this->getMaxSpeed() );
    this->is_closed = false;
}

Road::~Road(){
}

QJsonObject Road::toJSON(){
    QJsonObject json = Agent::toJSON();
    QJsonObject properties = json["properties"].toObject();
    properties.insert("road_type" , AgentToUI::formatProperty( RoadConstants::roadTypeToString( this->getRoadType() ) ) );
    properties.insert("max_speed" , AgentToUI::formatProperty( this->max_speed ) );
    properties.insert("length" , AgentToUI::formatProperty( this->length ) );
    properties.insert("is_closed" , AgentToUI::formatProperty( this->is_closed ) );
    json.insert("properties" , properties);
    return json;
}

void Road::fromJSON(QJsonObject json){
    Agent::fromJSON(json);
    if( json.contains( "properties" ) ){
        QJsonObject properties = json["properties"].toObject();
        if( properties.contains("max_speed") ){ this->max_speed = properties["max_speed"].toDouble( 0.1 ); }
        if( properties.contains("length") ){ this->length = properties["length"].toDouble( 0.1 ); }
        if( properties.contains("is_closed") ){ this->is_closed = properties["is_closed"].toBool( false ); }
    }
}

/**********************************************************************
 GETTERS
**********************************************************************/

RoadConstants::road_types Road::getRoadType(){
    return this->road_type;
}

/**
 * @brief Road::getGradient Get the roads gradient
 * (positive for climbing up and negative for going down)
 * @return
 */
double Road::getGradient(){
    if( this->getLength() ){
        try {
            double h = this->getStartNode()->getCoordinate().z - this->getEndNode()->getCoordinate().z;
            h = ( h / this->getLength() ) * 100.0;
            // TODO FIX return h;
            return qMin( 6.0 , qMax( h , -6.0 ) );
        } catch(...){}
    }
    return 0;
}

double Road::getMaxSpeed(){
    return this->max_speed;
}

/**
 * @brief GraphEdge::getLength Get the edges length IN METERS!
 * @return
 */
double Road::getLength(){
    return this->length;
}

bool Road::isClosed(){
    return this->is_closed;
}

double Road::getCost(){
    return this->getLength() / this->getMaxSpeed();
}

/**********************************************************************
 SETTERS
**********************************************************************/

void Road::setRoadType(RoadConstants::road_types road_type){
    this->road_type = road_type;
}

void Road::setGeometry(Geometry *geom){
    // A roads geometry must be a LineString
    LineString* line = dynamic_cast<LineString*>(geom);
    Agent::setGeometry( geom );
    this->setLength( geom->getLength() );
}

void Road::setMaxSpeed(double max_speed){
    this->max_speed = max_speed;
    this->setCost( this->getLength() / this->getMaxSpeed() );
}

void Road::setClosed(bool closed){
    this->is_closed = closed;
}

/**
 * @brief Road::setLength Set the road length IN METERS!
 * @param length
 */
void Road::setLength(double length){
    this->length = length;
    this->setCost( this->getLength() / this->getMaxSpeed() );
}

/**********************************************************************
 METHODS
**********************************************************************/

