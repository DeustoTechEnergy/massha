#include "Person.h"

Person::Person(PersonConstants::gender gender, QDateTime born_date, QString class_name) : MovingAgent(class_name){
    this->gender = gender;
    this->setBornDatetime(born_date);
    this->getStyle()->icon_url = "http://cdn.flaticon.com/png/32/17447.png";
}

Person::~Person(){
}

QJsonObject Person::toJSON(){
    QJsonObject json = MovingAgent::toJSON();
    QJsonObject properties = json["properties"].toObject();
    properties.insert("born_date" , AgentToUI::formatProperty( this->getBornDatetime() ) );
    properties.insert("gender" , AgentToUI::formatProperty( this->getGender() ) );
    json.insert("properties" , properties);
    return json;
}

void Person::fromJSON(QJsonObject json){
    MovingAgent::fromJSON(json);
    if( json.contains( "properties" ) ){
    QJsonObject properties = json["properties"].toObject();
    }
}

/**********************************************************************
 GETTERS
**********************************************************************/

PersonConstants::gender Person::getGender(){
    return this->gender;
}

Agent* Person::rememberByName(QString agent_name){
    if ( this->names_memory.keys().contains(agent_name) ){
         return this->names_memory.value( agent_name , 0 );
    }
    return 0;
}

Agent* Person::rememberById(int id){
    foreach(QString class_name, this->agents_memory.keys()){
        Agent* agent = this->agents_memory[ class_name ].value( id , 0 );
        if (agent){
            return agent;
        }
    }
    return 0;
}

QList<Agent*> Person::rememberByClass(QString class_name){
    if ( this->agents_memory.keys().contains( class_name ) ){
         return this->agents_memory.value( class_name ).values();
    }
    QList<Agent*> result;
    return result;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void Person::memorize(Agent *agent){

    // If it is the first agent of this class we receive, create the class in the map
    if( !this->agents_memory.keys().contains( agent->getClassName() ) ){

        // Insert new map with the agents class
        QMap<unsigned int, Agent*> map;
        this->agents_memory.insert( agent->getClassName(), map );
    }

    // Add to memory
    this->agents_memory[ agent->getClassName() ].insert( agent->getId(), agent );
    this->names_memory.insert( agent->objectName() , agent );
}


void Person::forget(Agent *agent){
    this->agents_memory[ agent->getClassName() ].remove( agent->getId() );
    this->names_memory.remove( agent->objectName() );
}
