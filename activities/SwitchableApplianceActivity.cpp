#include "SwitchableApplianceActivity.h"
#include "environment/Environment.h"

SwitchableApplianceActivity::SwitchableApplianceActivity(ActivityConstants::activity_types type) : Activity( type ){
    this->attendance = ActivityConstants::ATTENDED;
    this->appliance = 0;
}

/**********************************************************************
 GETTERS
**********************************************************************/

GraphNode* SwitchableApplianceActivity::getGraphNode(){
    if( !this->appliance && !this->getSensorName().isEmpty() ){
        Agent* a = Environment::getInstance()->getAgent( this->getSensorName() );
        if( a ){ this->appliance = dynamic_cast<SwitchableAppliance*>( a ); }
    }
    if( this->appliance ){
        return this->appliance->getGraphNode();
    }
    return 0;
}

/**********************************************************************
 METHODS
**********************************************************************/

bool SwitchableApplianceActivity::applianceIsOccupied(){
    if( this->appliance ){
        return this->appliance->isRunning();
    }
    return false;
}

/**********************************************************************
 SLOTS
**********************************************************************/

void SwitchableApplianceActivity::startActivity(){

    // Set finished to 0
    this->finished_amount = 0;

    // Emit started signal
    emit this->activityStartedSignal( this );

    // Log activity started
    this->logStart();

    // Check if we have microwave
    if( !this->appliance ){
        qWarning() << "[SwitchableApplianceActivity::startActivity] Activity" << ActivityConstants::activityTypeToString( this->getType() ) << "no appliance" << this->sensor_name << "to use.";
        QTimer::singleShot( 0 , this ,  SLOT( finishActivity() ) );
        return;
    }

    // If appliance is being used by someone else, try again in 5 seconds
//   if( this->appliance->isRunning() ){
//        qDebug() << "[SwitchableApplianceActivity::startActivity] Appliance" << this->appliance->objectName() << "is in use, waiting for it to finish.";
//        QTimer::singleShot( TimeController::secondsToEnvironmentMseconds( 5 ) , this ,  SLOT( startActivity() ) );
//        return;
//    }

    // Start appliance and when we are finished, stop it
    QTimer::singleShot( TimeController::secondsToEnvironmentMseconds( this->estimated_duration ) , this , SLOT(finishActivity()) );
    Environment::getInstance()->getTimer()->singleShot( 0 , this->appliance , SLOT(run()) );
}

void SwitchableApplianceActivity::finishActivity(){

    // Stop the appliance
    if( this->appliance ){
        Environment::getInstance()->getTimer()->singleShot( 0 , this->appliance , SLOT(stop()) );
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
