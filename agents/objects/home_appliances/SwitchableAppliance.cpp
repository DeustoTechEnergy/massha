#include "SwitchableAppliance.h"

#include "utils/exporters/logger/Logger.h"

SwitchableAppliance::SwitchableAppliance(QString class_name) : Agent( class_name ){
}

/**********************************************************************
  SLOTS
 **********************************************************************/

void SwitchableAppliance::agentStarted(){
    Agent::agentStarted();
    this->started_time = TimeController::getCurrentDateTime();
}

