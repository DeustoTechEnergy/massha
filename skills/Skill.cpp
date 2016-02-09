#include "Skill.h"

#include "agents/Agent.h"

Skill::Skill(QObject* parent) : QObject(parent){
}

Skill::~Skill(){
}

/**********************************************************************
 GETTERS
**********************************************************************/

QList<SkillConstants::capabilities> Skill::getSkillActions(){
    return this->actions;
}
