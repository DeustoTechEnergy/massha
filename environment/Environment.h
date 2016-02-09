#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QCoreApplication>

#include "agents/Agent.h"
#include "environment/time/TimeController.h"
#include "environment/physical_environment/PhysicalEnvironment.h"
#include "environment/social_environment/SocialEnvironment.h"

class Environment : public Agent , public PhysicalEnvironment, public SocialEnvironment
{
    Q_OBJECT
public:
    ~Environment();
    static Environment* getInstance();
    virtual QJsonObject toJSON();
    void fromJSON(QJsonObject json);

    // GETTERS
    QList<QString> getAgentClasses();
    Agent* getAgent(QString agent_name); // Get one agent by its name
    Agent* getAgent(QString class_name, unsigned int id); // Get one agent
    Agent* getAgent(unsigned int id); // Get one agent
    QList<Agent*> getAgents(QString class_name); // Get only one class type agents
    bool containsAgent(Agent* agent);
    Point* getRandomPoint( Geometry* bounds = 0, int seed = qrand() );

    // SETTERS
    void setEnvironmentTime(QDateTime environmentTime, double time_speed);
    bool addAgent(Agent *agent);
    bool addAgents(QList<Agent*> agents);
    Geometry* updateAgentGeometry(Agent* agent, Geometry* old_geom, Geometry* new_geom);
    bool removeAgent(Agent* agent);
    bool removeAgent(unsigned int id);    

private:
    Environment();
    Environment(Environment const&);

    // QCoreApplication
    QCoreApplication * app;

    // Environment elements
    TimeController * environment_date_time;

    /**
     * ENVIRONMENT MEMORY
     */
    QMap<QString, QMap<unsigned int, Agent*> > environment_agents; // QMAP<ClassName, QMAP< ID , AGENT>>
    QMap<QString , Agent*> environment_agent_names; // QMAP<AgentName, AGENT>

    // Mutex, for avoiding concurrency
    QMutex mutex;

};

#endif // ENVIRONMENT_H
