#ifndef ACTIVITYPERSON_H
#define ACTIVITYPERSON_H

#include <QObject>

#include "agents/people/Person.h"
#include "activities/ActivityConstants.h"
#include "activities/ActivityCreator.h"
#include "activities/Activity.h"

#include "skills/graph/DijkstraSkill.h"

/**
 * @brief The ActivityPerson class
 * Person that selects one random activity from its to_do_list, starts it and waits for it to finish.
 * Once it has finished, it selects another one from the to_do_list untill it has completed them all.
 */
class ActivityPerson : public Person
{
    Q_OBJECT
public:
    ActivityPerson(PersonConstants::gender gender = PersonConstants::MALE, QDateTime born_date = TimeController::getCurrentDateTime(), QString class_name = "ActivityPerson");
    virtual QJsonObject toJSON();

    // GETTERS
    QList<Activity*> getToDoActivities();
    QList<Activity*> getDoneActivities();

    // SETTERS
    void setToDoActivities( QList<Activity*> activities );
    void addToDoActivity( Activity* activity );

    // METHODS
    Activity* getDoneActivity( ActivityConstants::activity_types type );
    Activity* getToDoActivity( ActivityConstants::activity_types type );
    Activity* selectBestActivity( QList<Activity*> activities );

public slots:
    // Start the agents behaviour
    virtual void agentStarted();
    virtual void selectNextActivity();
    virtual void walkToSelectedActivity();
    virtual void startSelectedActivity();
    virtual void activityEnded( Activity* finished_activity );
    virtual void agentEnded();

private:

    /**
     * @brief to_do_list
     * List of Activities the agent needs to do.
     **/
    QList<Activity*> to_do_list;

    /**
     * @brief done_list
     * List of ALL the activities the agent has done. To keep track of all.
     */
    QList<Activity*> done_list;

    /**
     * @brief running_list
     * List of ALL the activities that are still running.
     */
    QList<Activity*> running_list;

    /**
     * @brief selected_activity
     * Selected next activity pointer, to keep a reference while jumping through the slots.
     */
    Activity* selected_activity;

};

#endif // ACTIVITYPERSON_H
