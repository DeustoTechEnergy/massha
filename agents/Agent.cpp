#include "Agent.h"

#include "environment/Environment.h"
#include "environment/time/TimeController.h"
#include "utils/exporters/geometry_to_geojson/GeometryToGeoJson.h"
#include "utils/importers/geojson_to_geometry_importer/GeoJsonToGeometry.h"
#include "utils/paralelism_controller/ParallelismController.h"

unsigned int Agent::counter = 0;

Agent::Agent(QString class_name, QObject* parent) : id(++Agent::counter) , class_name(class_name) , QObject(parent)
{
    this->born_datetime = QDateTime::currentDateTime();
    this->geometry = 0;
    this->node = 0;
    this->setObjectName( QString("%1-%2").arg( this->getClassName() ).arg( this->getId() ) );
    this->style = new UiStyle();
    this->running = false;
    this->timer = 0;
}

Agent::~Agent() {
    if( this->running ){ this->getTimer()->singleShot( 0 , this , SLOT(agentEnded()) ); }
    if( this->geometry ){ delete this->geometry; }
    if( this->node ){ delete this->node; }
    if( this->timer ){ this->timer->deleteLater(); }
    this->style->deleteLater();
}

/**
 * @brief Agent::toJSON Called when asked for many agents.
 * So only contains basic attributes and style.
 * Can be overwritten if want to add extra variables to the exporters
 * @return
 */
QJsonObject Agent::toMiniJSON(){
    QJsonObject properties;
    properties.insert("id" , AgentToUI::formatProperty( this->getId() ) );
    properties.insert("class", AgentToUI::formatProperty( this->getClassName() ) );
    properties.insert("name", AgentToUI::formatProperty( this->objectName() ) );
    properties.insert("style", this->style->toJSON() );
    return GeometryToGeoJson::format( this->getGeometry() , properties );
}

/**
 * @brief Agent::toExtendedJSON Called when asked for more info about this agent.
 * Contains all the additional information of the agent.
 * Can be overwritten if want to add extra variables to the exporters
 * @return
 */
QJsonObject Agent::toJSON(){
    return this->toMiniJSON();
}

/**
 * @brief Agent::fromJSON Update agent from JsonObject sent from frontend or importer
 * @param json {geometry : {} , properties : {}}
 */
void Agent::fromJSON(QJsonObject json){
    QJsonObject properties = json["properties"].toObject();
    if( properties.contains("name") ){ this->setObjectName( properties["name"].toObject()["value"].toString() ); }
    if( properties.contains("style") ){ this->style->fromJSON( properties["style"].toObject() ); }
    if( json.contains( "geometry" ) ){
        Geometry* geometry = GeoJsonToGeometry::format( json["geometry"].toObject() );
        if( geometry != 0 && geometry->isValid() ){
            this->setGeometry( geometry );
        }
    }
}

/**********************************************************************
 GETTERS
**********************************************************************/

unsigned int Agent::getId(){
    return this->id;
}

QString Agent::getClassName(){
    return this->class_name;
}

bool Agent::isRunning(){
    return this->running;
}

geos::geom::Geometry* Agent::getGeometry(){
    return this->geometry;
}

GraphNode* Agent::getGraphNode(){
    return this->node;
}

QList<SkillConstants::capabilities> Agent::getCapabilities(){
    return this->capabilities;
}

QDateTime Agent::getBornDatetime(){
    return this->born_datetime;
}

QTimer* Agent::getTimer(){
    return this->timer;
}

UiStyle* Agent::getStyle(){
    return this->style;
}


/**********************************************************************
 SETTERS
**********************************************************************/

/**
 * @brief GraphNode::setGeometry
 * WARNING, agent will take ownership of the geometry, Do not use
 * shared geometries, make a clone before passing it to agent!!
 * @param geom
 */
void Agent::setGeometry(geos::geom::Geometry* geometry){
    if( this->geometry != geometry ){
        this->geometry = Environment::getInstance()->updateAgentGeometry(this, this->geometry , geometry);
    }
}

void Agent::setGraphNode(GraphNode* node){
    this->node = node;
}

void Agent::setBornDatetime(QDateTime born_datetime){
    this->born_datetime = born_datetime;
}

void Agent::setStyle(UiStyle* style){
    this->style->deleteLater();
    this->style = style;
}

/**********************************************************************
 SLOTS
**********************************************************************/

void Agent::run(){

    if( this->running ){
        return;
    }

    QThread* thread = ParallelismController::getInstance()->getNewThread( this->id );
    this->moveToThread( thread );
    if( !this->timer ){ this->timer = new QTimer(); }
    this->timer->singleShot( 0 , this , SLOT( agentStarted() ));
}

void Agent::agentStarted(){
    this->running = true;
    emit agentStartedSignal();
    emit updateUISignal( AgentToUI::formatAgent( this , AgentToUI::CREATE ) );
}

void Agent::stop(){

    if( this->running ){
        this->timer->singleShot( 0 , this , SLOT(agentEnded()) );
    }
}

void Agent::agentEnded(){
    this->moveToThread( ParallelismController::getInstance()->liberateThread( this->thread() ) );
    this->running = false;
    emit agentEndedSignal();
    emit updateUISignal( AgentToUI::formatAgent( this , AgentToUI::DELETE ) );
}

