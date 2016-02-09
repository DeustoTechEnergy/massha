#include "MovingAgent.h"
#include "environment/Environment.h"
#include "environment/time/TimeController.h"

MovingAgent::MovingAgent(QString class_name) : Agent(class_name){

    // Speed
    this->moving_speed = 1.4; //m/s ~ 5km
    // Route
    this->moving_current_edge = 0;

}

MovingAgent::~MovingAgent(){
}

/**********************************************************************
 GETTERS
**********************************************************************/

GraphEdge* MovingAgent::getCurrentEdge(){
    return this->moving_current_edge;
}

double MovingAgent::getMovingSpeed(){
    return this->moving_speed;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void MovingAgent::setFromAndTo( GraphNode* from, GraphNode* to ){
    this->moving_from = from;
    this->moving_to = to;
}

void MovingAgent::setMovingSpeed(double speed){
    this->moving_speed = speed;
}

/**********************************************************************
 METHODS
**********************************************************************/

QList<GraphEdge*> MovingAgent::calculateRoute(GraphNode *from, GraphNode *to ){

    QList<GraphEdge*> route;

    DijkstraSkill* routing_skill = new DijkstraSkill();
    routing_skill->createGraph( Environment::getInstance()->getPhysicalGraph() );
    foreach(GraphEdge* e , routing_skill->dijkstraShortestPath( from , to ) ){
        route.append( e );
    }
    routing_skill->deleteLater();

    return route;
}

/**********************************************************************
 SLOTS
**********************************************************************/

/**
 * @brief RoutingPerson::start Calculate route and start driving from starting Node to ending Node
 */
void MovingAgent::agentStarted(){
    Agent::agentStarted();

    // Calculate route
    this->moving_route = this->calculateRoute( this->moving_from , this->moving_to );

    this->getTimer()->singleShot( 0 , this, SLOT(movingStarted()) );
}

void MovingAgent::movingStarted(){
    this->getTimer()->singleShot( 0 , this, SLOT(moveToNextEdge()) );
}

void MovingAgent::moveToNextEdge(){

    // No route
    if( this->moving_route.isEmpty() ){
        qDebug() << "[MovingAgent::moveToNextEdge(] Route is empty";
        this->getTimer()->singleShot( 0 , this, SLOT(movingEnded()) );
        return;
    }

    // No speed
    if( this->moving_speed <= 0 ){
        qDebug() << "[MovingAgent::moveToNextEdge(] Agent has no speed";
        this->getTimer()->singleShot( 0 , this, SLOT(movingEnded()) );
        return;
    }

    int edge_position = -1;
    if( this->moving_current_edge ){
        edge_position = this->moving_route.indexOf( this->moving_current_edge );
    }

    // Route finished
    if( edge_position >= this->moving_route.size()-1 ){
        this->getTimer()->singleShot( 0 , this, SLOT(movingEnded()) );
        return;
    }

    // Try to enter next road
    GraphEdge* next_edge = this->moving_route.at( edge_position+1 );
    this->moving_current_edge = next_edge;
    this->setGeometry( Environment::getInstance()->getGeometryFactory()->createPoint( this->moving_current_edge->getCoordinate() ) );

    // Update
    emit updateUISignal( AgentToUI::formatAgent( this , AgentToUI::UPDATE) );

    double seconds = this->moving_current_edge->getLength() / this->moving_speed;
    this->getTimer()->singleShot(  TimeController::secondsToEnvironmentMseconds( seconds ) , this , SLOT(moveToNextEdge()) );
}

void MovingAgent::movingEnded(){
    // Not any more in road
    if( this->moving_current_edge ){ this->moving_current_edge = 0; }
    this->getTimer()->singleShot( 0 , this, SLOT(agentEnded()) );
}

void MovingAgent::agentEnded(){
    Agent::agentEnded();
}
