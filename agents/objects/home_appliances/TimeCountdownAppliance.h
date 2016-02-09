#ifndef TIMECOUNTDOWNAPPLIANCE_H
#define TIMECOUNTDOWNAPPLIANCE_H

#include "agents/Agent.h"
#include "environment/time/TimeController.h"

/**
 * @brief The TimeCountdownAppliance class
 * Toaster / Microwave / Oven / ...
 */
class TimeCountdownAppliance : public Agent
{
    Q_OBJECT
public:
    TimeCountdownAppliance(QString class_name = "TimeCountdownAppliance");

    // GETTERS
    //QObject* getFromInside();
    double getRemainingSecondsToFinish();

    // SETTERS
    void setCountdownTime( double seconds ); // IN SECONDS
    //void putInside( QObject* object );

signals:
    void timeSetSignal();

protected slots:

    // To be implemented by children when the agents behaviour starts and ends
    virtual void agentStarted();

private:
    QDateTime started_time;
    double countdown_time;
    //QObject* inside;
};

#endif // TIMECOUNTDOWNAPPLIANCE_H
