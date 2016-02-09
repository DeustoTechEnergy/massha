#include <QJsonDocument>
#include <QJsonObject>

#include "TimeController.h"
#include "utils/exporters/agent_to_ui/AgentToUI.h"

// Static variables
QDateTime TimeController::environment_time = QDateTime::currentDateTime();
double TimeController:: time_speed = 1;
QDateTime TimeController::internal_start_time = QDateTime::currentDateTime();

TimeController* TimeController::getInstance(){
    static TimeController instance;
    return &instance;
}

TimeController::TimeController(){
}

QJsonObject TimeController::toJSON(){
    QJsonObject json;
    json.insert("datetime", AgentToUI::formatProperty( this->getCurrentDateTime() ) );
    json.insert("speed", AgentToUI::formatProperty( this->getTimeSpeed() , "x" ) );
    return json;
}

void TimeController::fromJSON(QJsonObject json){
    if( json.contains("speed") ){
        this->setTimeSpeed( qMax( json["speed"].toObject()["value"].toDouble() , 0.1) );
    }
}

/**********************************************************************
 SETTERS
**********************************************************************/

void TimeController::initTime(QDateTime environmentTime, double time_speed){
    TimeController::environment_time = environmentTime;
    TimeController::time_speed = time_speed;
    // Time starts counting from now
    TimeController::internal_start_time = QDateTime::currentDateTime();
}

void TimeController::setTimeSpeed(double time_speed){
    TimeController::environment_time = TimeController::getCurrentDateTime();
    TimeController::time_speed = qMax(0.1 , time_speed); // Avoid time_speed = 0
    // Time starts counting from now
    TimeController::internal_start_time = QDateTime::currentDateTime();
}

/**********************************************************************
   GETTERS
**********************************************************************/

double TimeController::msecondsToEnvironmentMseconds(double mseconds){
    if(TimeController::time_speed == 0){
        qWarning() << "[TimeController::msecondsToEnvironmentMseconds] Time speed is 0";
        return 1;
    }
    return qMax( 1.0 , (mseconds / TimeController::time_speed) );
}

double TimeController::secondsToEnvironmentMseconds(double seconds){
    if(TimeController::time_speed == 0){
        qWarning() << "[TimeController::secondsToEnvironmentMseconds] Time speed is 0";
        return 1;
    }
    return qMax( 1.0 , (seconds * 1000.0 / TimeController::time_speed) );
}

QDateTime TimeController::getCurrentDateTime(){
    quint64 spent_mseconds = TimeController::internal_start_time.msecsTo(QDateTime::currentDateTime());
    QDateTime calculated_time = TimeController::environment_time.addMSecs(spent_mseconds * TimeController::time_speed);
    return calculated_time;
}

double TimeController::getTimeSpeed(){
    return TimeController::time_speed;
}
