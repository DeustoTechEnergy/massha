#ifndef AGENTCONTAINSKILL_H
#define AGENTCONTAINSKILL_H

#include "QMutex"

#include "skills/Skill.h"
#include "agents/Agent.h"

/**
 * @brief The AgentContainSkill class
 * This skill makes entities available to contain other agents.
 * It will be used for roads, pipelines, buildings, etc.
 */
class AgentContainSkill : public Skill
{
        Q_OBJECT
public:
    AgentContainSkill();
    ~AgentContainSkill();

    // Getters
    bool isFull();
    bool isAgentInside( Agent* agent );
    unsigned int getMaxCapacity();
    unsigned int getAmountInside();
    QList<Agent*> getAgentsInside();

    // Setters
    void setMaxCapacity(unsigned int capacity);

    // SKILL METHODS
    bool enter(Agent* agent);
    bool exit(Agent* agent);
    void clear();

private:
    unsigned int max_capacity;
    QList<Agent*> agents_inside;
    // Mutex
    QMutex mutex;
};

#endif // AGENTCONTAINSKILL_H
