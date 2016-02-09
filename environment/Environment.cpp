#include "Environment.h"

#include "utils/exporters/geometry_to_geojson/GeometryToGeoJson.h"
#include "utils/http_server/HttpServer.h"
#include "utils/websocket_server/WebSocketServer.h"

/**
 * @brief Environment::getInstance
 * @return
 */
Environment* Environment::getInstance(){
    static Environment instance;

    // Create HTTP and WS servers
    WebSocketServer::getInstance();
    HttpServer::getInstance();

    return &instance;
}

/**
 * @brief Environment::Environment
 */
Environment::Environment() : Agent("Environment") , PhysicalEnvironment() , SocialEnvironment()
{
    // Init environment app
    //this->app = new QCoreApplication( 0 , {} );

    // Init environment DateTime
    this->environment_date_time = TimeController::getInstance();
    this->environment_date_time->initTime( this->getBornDatetime() );
    QThread environment_date_time_thread;
    this->environment_date_time->moveToThread( &environment_date_time_thread );

    // Random generators
    qsrand( QDateTime::currentDateTime().time().second() );

    // Style
    this->getStyle()->color = QColor(255,255,255,0);

    qDebug() << "[Environment::Environment] Environment created" << endl;
}

Environment::~Environment(){
    qDebug() << "[Environment::~Environment] Environment deleted" << endl;
}

QJsonObject Environment::toJSON(){
    QJsonObject json = Agent::toJSON();
    QJsonObject properties = json["properties"].toObject();
    QJsonArray agent_classes;
    foreach ( QString class_name, this->getAgentClasses() ) {
        agent_classes.append( class_name );
    }
    properties.insert("agent-classes", agent_classes );
    properties.insert( "time" , this->environment_date_time->toJSON() );
    properties.insert( "graph" , this->physicalToJSON() );
    json.insert("properties" , properties);
    return json;
}

void Environment::fromJSON(QJsonObject json){
    Agent::fromJSON(json);
    QJsonObject properties = json["properties"].toObject();
    if( properties.contains( "time" ) ){
        this->environment_date_time->fromJSON( properties["time"].toObject() );
    }
}

/**********************************************************************
 GETTERS
**********************************************************************/

Point* Environment::getRandomPoint(Geometry* bounds, int seed){
    return PhysicalEnvironment::getRandomPoint(bounds ? bounds : this->getGeometry(), seed);
}

/**********************************************************************
 SETTERS
**********************************************************************/

void Environment::setEnvironmentTime(QDateTime environmentTime, double time_speed){
    this->environment_date_time->initTime(environmentTime, time_speed);
}

/**********************************************************************
 AGENTS METHODS
**********************************************************************/
/* CREATE - DELETE
**********************************************************************/

bool Environment::addAgents(QList<Agent*> agents){
    QListIterator<Agent*> i(agents);
    bool inserted = true;
    while(i.hasNext()){
        // Call for each agent
        inserted = inserted && this->addAgent( i.next() );
    }
    return inserted;
}

bool Environment::addAgent(Agent *agent){

    {
        QMutexLocker locker(&this->mutex);

        // If it is the first agent of this class we receive, create the class in the hashmap
        if( !this->environment_agents.keys().contains(agent->getClassName()) ){

            // Insert new map with the agents class
            QMap<unsigned int, Agent*> map;
            this->environment_agents.insert(agent->getClassName(), map);

            // Create urls in http_server
            HttpServer::getInstance()->createUrls( agent->getClassName() );
        }
    }

    // Add to agents map
    this->environment_agents[ agent->getClassName() ].insert( agent->getId(), agent );
    this->environment_agent_names.insert( agent->objectName() , agent );

    // Add to physical environment
    PhysicalEnvironment::addAgent( agent );

    // Add to communication environment
    SocialEnvironment::addAgent( agent );

    // Connect to websocket_server
    this->connect( agent , SIGNAL(updateUISignal(QByteArray)) , WebSocketServer::getInstance() , SLOT(push(QByteArray)) );

  return true;
}

bool Environment::removeAgent(unsigned int id){

    Agent* remove_agent = this->getAgent(id);
    if (remove_agent){
        this->environment_agents[remove_agent->getClassName()].remove( remove_agent->getId() );
        this->environment_agent_names.remove( remove_agent->objectName() );

        // Delete from physical environment
        PhysicalEnvironment::removeAgent(remove_agent);

        // Delete from communication environment
        // NONE

        return true;
    }
    return false;
}

/* UPDATE
**********************************************************************/

/**
 * @brief Environment::updateAgentGeometry Given the old and new geometry returns the
 * new geometry the agent should have.
 * WARNING the two given geometries will be deleted so never share geometries amont agents
 * @param agent
 * @param old_geom
 * @param new_geom
 * @return
 */
Geometry* Environment::updateAgentGeometry(Agent* agent, Geometry* old_geom, Geometry* new_geom){
    // Check constraints

    // If it does not belong to the environment, do not update in the spatial index
    if ( this->containsAgent(agent) ){
        return PhysicalEnvironment::updateAgentGeometry(agent , old_geom , new_geom);
    }

    // If agent is not in the environment, it can have the geometry it wants
    Geometry* geometry = new_geom->clone();

    if( old_geom ){ delete old_geom; }
    if( new_geom ){ delete new_geom; }

    return geometry;
}

/* GETTERS
**********************************************************************/

bool Environment::containsAgent(Agent* agent){
    if ( !this->environment_agents.keys().contains(agent->getClassName()) ){
        return false;
    }
    return this->environment_agents[ agent->getClassName() ].contains( agent->getId() );
}

QList<QString> Environment::getAgentClasses(){
    return this->environment_agents.keys();
}

Agent* Environment::getAgent(QString agent_name){
    if ( this->environment_agent_names.keys().contains(agent_name) ){
         return this->environment_agent_names.value(agent_name , 0);
    }
    return 0;
}

Agent* Environment::getAgent(QString class_name, unsigned int id){
    if ( this->environment_agents.keys().contains(class_name) ){
         return this->environment_agents.value(class_name).value(id , 0);
    }
    return 0;
}

Agent* Environment::getAgent(unsigned int id){
    foreach(QString class_name, this->environment_agents.keys()){
        Agent* agent = this->getAgent(class_name , id);
        if (agent){
            return agent;
        }
    }
    return 0;
}

QList<Agent*> Environment::getAgents(QString class_name){
    return this->environment_agents[class_name].values();
}
