#ifndef PERSON_H
#define PERSON_H

#include "agents/moving/MovingAgent.h"
#include "agents/people/PersonConstants.h"
#include "environment/time/TimeController.h"

class Person : public MovingAgent
{
    Q_OBJECT
public:
    Person(PersonConstants::gender gender = PersonConstants::MALE, QDateTime born_date = TimeController::getCurrentDateTime(), QString class_name = "Person");
    ~Person();
    virtual QJsonObject toJSON();
    virtual void fromJSON(QJsonObject json);

    // GETTERS
    PersonConstants::gender getGender();
    Agent* rememberByName(QString agent_name);
    Agent* rememberById(int id);
    QList<Agent*> rememberByClass(QString class_name);

    // SETTERS
    void memorize(Agent* agent);
    void forget(Agent* agent);

private:
    PersonConstants::gender gender;

    /**
     * PERSON MEMORY
    **/
    QMap<QString, QMap<unsigned int, Agent*> > agents_memory;  // QMAP<ClassName, QMAP< ID , AGENT>>
    QMap<QString , Agent*> names_memory;  // QMAP<AgentName, AGENT>

};

#endif // PERSON_H
