#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <QObject>
#include <QTimer>

#include "environment/time/TimeController.h"
#include "utils/exporters/logger/Logger.h"
#include "activities/ActivityConstants.h"
#include "agents/Agent.h"

/**
 * @brief The AgentActivity class
 * Groups some subactivities and adds priority, time restrictions and conditions
 */
class Activity : public QObject
{
    Q_OBJECT
public:

    Activity(ActivityConstants::activity_types type);
    ~Activity();

    // GETTERS
    ActivityConstants::activity_types getType();
    ActivityConstants::attendance getAttendance();
    double getEstimatedDuration();
    double getFrequency();
    double getPriorityBase();
    double getPriorityIncrement();
    QTime getDesiredStartTime();
    QTime getDesiredEndTime();
    QTime getMandatoryStartTime();
    QTime getMandatoryEndTime();
    Activity* getParentActivity();
    QList<Activity*> getSubactivities();
    QList<ActivityConstants::activity_types> getPreconditions();
    QList<ActivityConstants::activity_types> getOutcomes();
    QList<ActivityConstants::activity_types> getConsumes();
    double getFinishObjective();
    double getFinishIncrement();
    double getFinishedAmount();
    bool isFinished();
    Agent* getActivityDoer();
    virtual GraphNode* getGraphNode();
    QString getSensorName();

    // SETTERS
    void setEstimatedDuration(double seconds);
    void setAttendance(ActivityConstants::attendance attendance);
    void setFrequency(double frequency);
    void setPriorityBase(double priority_base);
    void setPriorityIncrement(double priority_increment);
    void resetPriorityCounter();
    void setDesiredStartTime(QTime desired_start_time);
    void setDesiredEndTime(QTime desired_end_time);
    void setMandatoryStartTime(QTime mandatory_start_time);
    void setMandatoryEndTime(QTime mandatory_end_time);
    void addPrecondition(ActivityConstants::activity_types t);
    void addOutcome(ActivityConstants::activity_types t);
    void addConsume(ActivityConstants::activity_types t);
    void setParentActivity(Activity* activity);
    void addSubactivity(Activity* subactivity);
    void setFinishObjective(double finish_objective);
    void setFinishIncrement(double finish_increment);
    void addFinishIncrement( double finish_increment );
    void setActivityDoer(Agent* agent);
    void setSensorName(QString sensor_name);

    // METHODS
    virtual bool applianceIsOccupied();
    bool hasMandatoryTime();
    bool isInMandatoryTime();
    bool hasDesiredTime();
    bool isInDesiredTime();
    double calculateRouletteValue();

    // To be implemented by children
    virtual void startActivity();

public slots:
    virtual void finishActivity();

signals:
    void activityStartedSignal( Activity* started_activity ); // Emitted when started
    void activityEndedSignal( Activity* finished_activity ); // Emitted when finished

protected:

    /**
      * Attributes
      */
    ActivityConstants::activity_types type;
    ActivityConstants::attendance attendance; // The person can do other subactivities/activities while this one is processed
    QString sensor_name;
    double frequency; // How much does this activity appear
    double estimated_duration; // In seconds
    Agent* activity_doer; // Agent that executes the activity

    /**
     * @brief Priority counter
     */
    double priority_base;
    QDateTime priority_start; // Activity creation datetime
    double priority_increment; // Increment per hour since the creation time


    /**
     * Activity time restrictions
     */
    QTime desired_start_time;
    QTime desired_end_time;
    QTime mandatory_start_time;
    QTime mandatory_end_time;

    /**
     * @brief preconditions
     * List of prior Activities that MUST be done by the time the current
     * Activity is carried out
     */
    QList<ActivityConstants::activity_types> preconditions;

    /**
     * @brief add_todo_when_finished
     * List of new Activities that are created whe the current Activity
     * is carried out.
     * For example, when a person has finished EATING_FOOD, they
     * must add a DO_THE_DISHES activity to their TO_DO list
     */
    QList<ActivityConstants::activity_types> outcomes;

    /**
     * @brief remove all ocurrences that match these types from done_list when this activity finishes
     * Remove Activities from done list
     */
    QList<ActivityConstants::activity_types> consumes;

    /**
      * Subactivities and parent activities
      */
    Activity* parent_activity; // THIS SUBACTIVITY'S PARENT
    QList<Activity*> subactivities; // SUBACTIVITIES

    /**
     * @brief Flags to know how much of this activity has been done and how much it represents to its parent
     */
    double finish_objective;// HOW MUCH OF THIS ACTIVITY NEEDS TO BE DONE, WHEN THE AMOUNT REACHES LIMIT = ACTIVITY DONE
    double finished_amount;// HOW MUCH OF THIS ACTIVITY HAS BEEN DONE, STARTS IN 0
    double finish_increment; // HOW MUCH WILL THIS SUBACTIVITY SUM TO ITS PARENTS PROCESS

    /**
     * @brief logStart and logEnd
     */
    void logStart();
    void logEnd();

};

#endif // ACTIVITY_H
