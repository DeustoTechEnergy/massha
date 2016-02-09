#ifndef TIMECOUNTDOWNACTIVITY_H
#define TIMECOUNTDOWNACTIVITY_H

#include "activities/Activity.h"
#include "agents/objects/home_appliances/TimeCountdownAppliance.h"

class TimeCountdownActivity : public Activity
{
    Q_OBJECT

public:
    TimeCountdownActivity( ActivityConstants::activity_types type );

    // GETTERS
    virtual GraphNode* getGraphNode();

    // METHODS
    virtual bool applianceIsOccupied();

public slots:
    virtual void startActivity();
    virtual void finishActivity();

private:
    TimeCountdownAppliance* appliance;
};

#endif // TIMECOUNTDOWNACTIVITY_H
