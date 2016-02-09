#include "TimeCountdownActivity.h"
#include "environment/Environment.h"

TimeCountdownActivity::TimeCountdownActivity(ActivityConstants::activity_types type) : Activity( type ){
    this->attendance = ActivityConstants::UNATTENDED;
    this->appliance = 0;
}

/**********************************************************************
 GETTERS
**********************************************************************/

GraphNode* TimeCountdownActivity::getGraphNode(){
    if( !this->appliance && !this->getSensorName().isEmpty() ){
        Agent* a = Environment::getInstance()->getAgent( this->getSensorName() );
        if( a ){ this->appliance = dynamic_cast<TimeCountdownAppliance*>( a ); }
    }
    if( this->appliance ){
        return this->appliance->getGraphNode();
    }
    return 0;
}

/**********************************************************************
 METHODS
**********************************************************************/

bool TimeCountdownActivity::applianceIsOccupied(){
    if( this->appliance ){
        return this->appliance->isRunning();
    }
    return false;
}

/**********************************************************************
 SLOTS
**********************************************************************/

void TimeCountdownActivity::startActivity(){

    // Set finished to 0
    this->finished_amount = 0;

    // Emit started signal
    emit this->activityStartedSignal( this );

    // Log activity started
    this->logStart();

    // Check if we have appliance
    if( !this->appliance ){
        qWarning() << "[TimeCountdownActivity::startActivity] Activity" << ActivityConstants::activityTypeToString( this->getType() ) << "no appliance" << this->sensor_name << "to use.";
        QTimer::singleShot( 0 , this ,  SLOT( finishActivity() ) );
        return;
    }

//    // If appliance is being used by someone else, try again in 5 seconds
//    if( this->appliance->isRunning() ){
//        qDebug() << "[TimeCountdownActivity::startActivity] appliance is in use, waiting for it to finish.";
//        QTimer::singleShot( TimeController::secondsToEnvironmentMseconds( 5 ) , this ,  SLOT( startActivity() ) );
//        return;
//    }

    // Start appliance and wait for it to finish
    this->appliance->setCountdownTime( this->estimated_duration );
    this->connect( this->appliance , SIGNAL( agentEndedSignal() ) , this , SLOT( finishActivity() ) );
    Environment::getInstance()->getTimer()->singleShot( 0 , this->appliance , SLOT(run()) );
}

void TimeCountdownActivity::finishActivity(){

    // Disconnect from object
    if( this->appliance ){
        this->disconnect( this->appliance , SIGNAL( agentEndedSignal() ) , this , SLOT( finishActivity() ) );
    }

    // Log activity ended
    this->logEnd();

    // Set finished to limit
    this->finished_amount = this->finish_objective;

    // If it has parent, IMPORTANT TO ADD THE INCREMENT BEFORE EMITING THE FINISH SIGNAL
    if( this->parent_activity ){
        this->parent_activity->addFinishIncrement( this->getFinishIncrement() );
    }

    // Emit finished signal
    emit this->activityEndedSignal( this );
}
