#ifndef SWITCHABLEAPPLIANCEACTIVITY_H
#define SWITCHABLEAPPLIANCEACTIVITY_H

#include "activities/Activity.h"
#include "agents/objects/home_appliances/SwitchableAppliance.h"

class SwitchableApplianceActivity : public Activity
{
    Q_OBJECT

public:
    SwitchableApplianceActivity( ActivityConstants::activity_types type );

    // GETTERS
    virtual GraphNode* getGraphNode();

    // METHODS
    virtual bool applianceIsOccupied();

public slots:
    virtual void startActivity();
    virtual void finishActivity();

private:
    SwitchableAppliance* appliance;
};

#endif // SWITCHABLEAPPLIANCEACTIVITY_H
