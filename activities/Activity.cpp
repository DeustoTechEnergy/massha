#include "Activity.h"
#include "environment/time/TimeController.h"

Activity::Activity(ActivityConstants::activity_types type) : QObject(){
    this->type = type;
    this->attendance = ActivityConstants::ATTENDED;
    this->frequency = 1;
    this->estimated_duration = 0;
    this->sensor_name = "";
    this->activity_doer = 0;

    // Priority
    this->priority_start = TimeController::getCurrentDateTime();
    this->priority_base = 0;
    this->priority_increment = 0;

    // Parents
    this->parent_activity = 0;

    // Finished flags
    this->finish_objective = 1;
    this->finished_amount = 0;
    this->finish_increment = 1;
}

Activity::~Activity(){
    // Delete all activity actions
    qDeleteAll( this->subactivities );
}

/**********************************************************************
 GETTERS
**********************************************************************/

ActivityConstants::activity_types Activity::getType(){
    return this->type;
}

ActivityConstants::attendance Activity::getAttendance(){
    return this->attendance;
}

double Activity::getFrequency(){
    return this->frequency;
}

double Activity::getEstimatedDuration(){
    return this->estimated_duration;
}

double Activity::getPriorityBase(){
    return this->priority_base;
}

double Activity::getPriorityIncrement(){
    return this->priority_increment;
}

QTime Activity::getDesiredStartTime(){
    return this->desired_start_time;
}

QTime Activity::getDesiredEndTime(){
    return this->desired_end_time;
}

QTime Activity::getMandatoryStartTime(){
    return this->mandatory_start_time;
}

QTime Activity::getMandatoryEndTime(){
    return this->mandatory_end_time;
}

Activity* Activity::getParentActivity(){
    return this->parent_activity;
}

QList<Activity*> Activity::getSubactivities(){
    return this->subactivities;
}

QList<ActivityConstants::activity_types> Activity::getPreconditions(){
    return this->preconditions;
}

QList<ActivityConstants::activity_types> Activity::getConsumes(){
    return this->consumes;
}

QList<ActivityConstants::activity_types> Activity::getOutcomes(){
    return this->outcomes;
}

double Activity::getFinishObjective(){
    return this->finish_objective;
}

double Activity::getFinishIncrement(){
    return this->finish_increment;
}

double Activity::getFinishedAmount(){
    return this->finished_amount;
}

bool Activity::isFinished(){
    return this->finished_amount >= this->finish_objective;
}

Agent* Activity::getActivityDoer(){
    return this->activity_doer;
}

QString Activity::getSensorName(){
    return this->sensor_name;
}

GraphNode* Activity::getGraphNode(){
    return 0;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void Activity::setEstimatedDuration(double seconds){
    this->estimated_duration = seconds;
}

void Activity::setAttendance(ActivityConstants::attendance attendance){
    this->attendance = attendance;
}

void Activity::setFrequency(double frequency){
    this->frequency = frequency;
}

void Activity::setPriorityBase(double priority_base){
    this->priority_base = priority_base;
}

void Activity::setPriorityIncrement(double increment){
    this->priority_increment = increment;
}

void Activity::resetPriorityCounter(){
    this->priority_start = TimeController::getCurrentDateTime();
}

void Activity::setDesiredStartTime(QTime desirable_start_time){
    this->desired_start_time = desirable_start_time;
}

void Activity::setDesiredEndTime(QTime desirable_end_time){
    this->desired_end_time = desirable_end_time;
}

void Activity::setMandatoryStartTime(QTime mandatory_start_time){
    this->mandatory_start_time = mandatory_start_time;
}

void Activity::setMandatoryEndTime(QTime mandatory_end_time){
    this->mandatory_end_time = mandatory_end_time;
}

void Activity::addPrecondition(ActivityConstants::activity_types t){
    this->preconditions.append( t );
}

void Activity::addOutcome(ActivityConstants::activity_types t){
    this->outcomes.append( t );
}

void Activity::addConsume(ActivityConstants::activity_types t){
    this->consumes.append( t );
}

void Activity::setParentActivity(Activity* activity){
    this->parent_activity = activity;
}

void Activity::addSubactivity(Activity* sub){
    this->subactivities.append( sub );
    sub->setParentActivity( this );
}

void Activity::setFinishObjective(double finish_objective){
    this->finish_objective = finish_objective;
}

void Activity::setFinishIncrement(double finish_increment){
    this->finish_increment = finish_increment;
}

void Activity::addFinishIncrement( double finish_increment ){

    // If it does not have mandatory end time, go incrementing its finished_amount
    if( !this->hasMandatoryTime() ){

        this->finished_amount += finish_increment;

        // IMPORTANT!! IF IT HAS JUST PASSED THE LIMIT THANKS TO THIS INCREMENT, NOTIFY ITS FINISH
        if( this->isFinished() && (this->finished_amount - finish_increment) < this->finish_objective ){
            this->finishActivity();
        }
    }

    // Else if it has mandatory end_time, do not finish untill its end time
    else {

        QTime current_time = TimeController::getCurrentDateTime().time();
        if( current_time > this->mandatory_end_time) {
            this->finishActivity();
        }
    }
}

void Activity::setActivityDoer(Agent *agent){
    this->activity_doer = agent;
}

void Activity::setSensorName(QString sensor_name){
    this->sensor_name = sensor_name;
}

/**********************************************************************
 SLOTS
**********************************************************************/

void Activity::startActivity(){
    // Generic StartActivityMethod for non implemented activities
    // Set finished to 0
    this->finished_amount = 0;

    // Emit started signal
    emit this->activityStartedSignal( this );

    // Log activity started
    this->logStart();

    // If it has no subactivities, execute it normally
    if ( this->subactivities.isEmpty() ){
        QTimer::singleShot( TimeController::secondsToEnvironmentMseconds( this->estimated_duration ), this ,  SLOT( finishActivity() ) );
        return;
    }

    // Else, it will have subactivities, so while subactivities go finishing, they will be adding finish_amount
    // and once reached the limit, this activity will finish
}

void Activity::finishActivity(){

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

/**********************************************************************
 METHODS
**********************************************************************/

bool Activity::applianceIsOccupied(){
    // Parent activities have no appliance in use
    return false;
}

bool Activity::hasMandatoryTime(){
    if( this->mandatory_start_time.isValid() && this->mandatory_end_time.isValid() ){
        return true;
    }
    return false;
}

bool Activity::isInMandatoryTime(){
    QTime current_time = TimeController::getCurrentDateTime().time();
    if( this->hasMandatoryTime() && current_time >= this->mandatory_start_time && current_time < this->mandatory_end_time ){
        return true;
    }
    return false;
}

bool Activity::hasDesiredTime(){
    if( this->desired_start_time.isValid() && this->desired_end_time.isValid() ){
        return true;
    }
    return false;
}

bool Activity::isInDesiredTime(){
    QTime current_time = TimeController::getCurrentDateTime().time();
    if( this->hasDesiredTime() && this->desired_start_time.isValid() && this->desired_end_time.isValid() && current_time > this->desired_start_time && current_time < this->desired_end_time ){
        return true;
    }
    return false;
}

double Activity::calculateRouletteValue(){

    // If today should not be done, return < 0

    // It it has mandatory time and it should not be done now, return < 0
    if( this->hasMandatoryTime() && !this->isInMandatoryTime() ){
        return -1;
    }

    double value = this->getFrequency();

    // Priority increment
    double increment = this->priority_increment * ( this->priority_start.secsTo( TimeController::getCurrentDateTime() ) / 3600.0 );
    value += this->priority_base + increment;

    // Desirable time
    if( this->isInDesiredTime() ){
        value *= 2;
    }

    return value;
}

void Activity::logStart(){
    Logger::log(
                QString("%1,%2,%3,%4,%5,%6,%7,%8")
                .arg( this->activity_doer->getId() )
                .arg( TimeController::getCurrentDateTime().toString("yyyy/MM/dd hh:mm:ss") )
                .arg( this->getParentActivity() ? this->sensor_name.split('_').at(1) : "" )
                .arg( this->sensor_name )
                .arg( this->getParentActivity() ? "ON" : "")
                .arg( this->getParentActivity() ? ActivityConstants::activityTypeToString( this->getParentActivity()->getType() ) : ActivityConstants::activityTypeToString( this->getType() ) )
                .arg( !this->getParentActivity() ? "START" : "")
                .arg( this->attendance ),
                this->activity_doer->objectName()
                );
}

void Activity::logEnd(){
    Logger::log(
                QString("%1,%2,%3,%4,%5,%6,%7,%8")
                .arg( this->activity_doer->getId() )
                .arg( TimeController::getCurrentDateTime().toString("yyyy/MM/dd hh:mm:ss") )
                .arg( this->getParentActivity() ? this->sensor_name.split('_').at(1) : "" )
                .arg( this->sensor_name )
                .arg( this->getParentActivity() ? "OFF" : "")
                .arg( this->getParentActivity() ? ActivityConstants::activityTypeToString( this->getParentActivity()->getType() ) : ActivityConstants::activityTypeToString( this->getType() ) )
                .arg( !this->getParentActivity() ? "END" : "")
                .arg( QString::number( this->estimated_duration ) ),
                this->activity_doer->objectName()
                );
}
