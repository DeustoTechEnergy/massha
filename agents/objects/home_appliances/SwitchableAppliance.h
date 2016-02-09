#ifndef SWITCHABLEAPPLIANCE_H
#define SWITCHABLEAPPLIANCE_H

#include "agents/Agent.h"
#include "environment/time/TimeController.h"

class SwitchableAppliance : public Agent
{
    Q_OBJECT
public:
    SwitchableAppliance(QString class_name = "SwitchableAppliance");

protected slots:

    // To be implemented by children when the agents behaviour starts and ends
    virtual void agentStarted();

private:
    QDateTime started_time;
};

#endif // SWITCHABLEAPPLIANCE_H
