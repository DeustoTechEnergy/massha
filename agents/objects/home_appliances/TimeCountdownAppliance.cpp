#include "TimeCountdownAppliance.h"

TimeCountdownAppliance::TimeCountdownAppliance(QString class_name) : Agent(class_name){
    this->countdown_time = 0;
}

/**********************************************************************
 GETTERS
**********************************************************************/

/*QObject* TimeCountdownAppliance::getFromInside(){
    return this->inside;
}*/

double TimeCountdownAppliance::getRemainingSecondsToFinish(){
    if( !this->isRunning() ){
        return 0;
    } else {
        return this->started_time.addSecs( this->countdown_time ).secsTo( TimeController::getCurrentDateTime() );
    }
}

/**********************************************************************
 SETTERS
**********************************************************************/

void TimeCountdownAppliance::setCountdownTime(double seconds){
   this->countdown_time = seconds;
   emit this->timeSetSignal();
}

/*void TimeCountdownAppliance::putInside(QObject *object){
    this->inside = object;
}*/

/**********************************************************************
 SLOTS
**********************************************************************/

void TimeCountdownAppliance::agentStarted(){
    Agent::agentStarted();
    this->started_time = TimeController::getCurrentDateTime();
    this->getTimer()->singleShot( TimeController::secondsToEnvironmentMseconds( this->countdown_time ) , this , SLOT( agentEnded() ) );
}
