#include "AgentContainSkill.h"
#include "environment/Environment.h"
#include "QMutexLocker"

AgentContainSkill::AgentContainSkill() : Skill(){
    this->max_capacity = 1;
}

AgentContainSkill::~AgentContainSkill(){
}

bool AgentContainSkill::enter(Agent* agent){

    // See if it is already inside
    if ( this->agents_inside.contains( agent ) ){
        return true;
    }

    {
        QMutexLocker locker(&mutex);
        if ( !this->isFull() ) {
            this->agents_inside.append(agent);
            return true;
        }
    }
    return false;
}

bool AgentContainSkill::exit(Agent* agent){
    QMutexLocker locker(&mutex);
    return this->agents_inside.removeAll( agent );
}

void AgentContainSkill::clear(){
    QMutexLocker locker(&mutex);
    this->agents_inside.clear();
}

/**********************************************************************
 GETTERS
**********************************************************************/

bool AgentContainSkill::isAgentInside( Agent* agent ){
    return this->agents_inside.contains( agent );
}

bool AgentContainSkill::isFull(){
    return this->agents_inside.size() >= int(this->max_capacity);
}

unsigned int AgentContainSkill::getMaxCapacity(){
    return this->max_capacity;
}

unsigned int AgentContainSkill::getAmountInside(){
    return this->agents_inside.size();
}

QList<Agent*> AgentContainSkill::getAgentsInside(){
    return this->agents_inside;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void AgentContainSkill::setMaxCapacity(unsigned int capacity){
    QMutexLocker locker(&mutex);
    this->max_capacity = capacity;
}
