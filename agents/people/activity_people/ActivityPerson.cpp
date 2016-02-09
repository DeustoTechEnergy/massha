#include "ActivityPerson.h"
#include "skills/random_generators/RouletteWheelSkill.h"
#include "environment/Environment.h"

ActivityPerson::ActivityPerson(PersonConstants::gender gender, QDateTime born_date, QString class_name) : Person( gender , born_date , class_name ){
    this->selected_activity = 0;
}

QJsonObject ActivityPerson::toJSON(){
    QJsonObject json = MovingAgent::toJSON();
    QJsonObject properties = json["properties"].toObject();
    properties.insert( "current_time" , AgentToUI::formatProperty( TimeController::getCurrentDateTime() ) );
    if( this->selected_activity ){
        if( this->selected_activity->getParentActivity() ){
            properties.insert( "current_activity" , AgentToUI::formatProperty( ActivityConstants::activityTypeToString( this->selected_activity->getParentActivity()->getType() ) ) );
            properties.insert( "current_action" , AgentToUI::formatProperty( ActivityConstants::activityTypeToString( this->selected_activity->getType() ) ) );
        } else {
            properties.insert( "current_activity" , AgentToUI::formatProperty( ActivityConstants::activityTypeToString( this->selected_activity->getType() ) ) );
        }
    }
    json.insert("properties" , properties);
    return json;
}

/**********************************************************************
 GETTERS
**********************************************************************/

QList<Activity*> ActivityPerson::getToDoActivities(){
    return this->to_do_list;
}

QList<Activity*> ActivityPerson::getDoneActivities(){
    return this->done_list;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void ActivityPerson::setToDoActivities(QList<Activity*> activities){
    this->to_do_list = activities;
}

void ActivityPerson::addToDoActivity(Activity* activity){
    this->to_do_list.append(activity);
}

/**********************************************************************
 METHODS
**********************************************************************/

Activity* ActivityPerson::getDoneActivity(ActivityConstants::activity_types type){
    foreach(Activity* a , this->done_list){
        if( a->getType() == type ){
            return a;
        }
    }
    return 0;
}

Activity* ActivityPerson::getToDoActivity(ActivityConstants::activity_types type){
    foreach(Activity* a , this->to_do_list){
        if( a->getType() == type ){
            return a;
        }
    }
    return 0;
}

Activity* ActivityPerson::selectBestActivity( QList<Activity*> activities ){

    // If suddenly we have an activity in TODO list which is mandatory, change to it.
    foreach( Activity* a , this->to_do_list ){
        if( a->isInMandatoryTime() ){
            qDebug() << "[ActivityPerson::selectBestActivity]" << TimeController::getCurrentDateTime().time() << "Mandatory activity found" << ActivityConstants::activityTypeToString( a->getType() ) << this->running_list.size();

            if ( this->selected_activity && this->running_list.contains( this->selected_activity )){
                this->running_list.removeAll( this->selected_activity );
            }
            return a;
        }
    }

    // Create roulette skill
    RouletteWheelSkill* roulette_skill = new RouletteWheelSkill();

    // Otherwise, get the best activity from the given list according to mandatory time, desirable, priority, etc.
    Activity* best_activity = 0;
    foreach( Activity* a , activities ){
        double val = a->calculateRouletteValue();
        if( val > 0 ){
            roulette_skill->addElement( a , val );
        }
    }

    best_activity = dynamic_cast<Activity*>( roulette_skill->getElement() );
    roulette_skill->deleteLater();

    return best_activity;
}

/**********************************************************************
 SLOTS
**********************************************************************/

void ActivityPerson::agentStarted(){    
    Agent::agentStarted();

    qDebug() << "[ActivityPerson::agentStarted] STARTING WITH TODO LIST" << this->to_do_list;

    this->getTimer()->singleShot(0 , this , SLOT( selectNextActivity()) );
}

/**
 * @brief ActivityPerson::selectNextActivity
 * IF this->selected_activity is 0 : it will get the best activity from TODO list
 * IF this->selected_activity has subactivities : it will get the best subactivity from its subactivities
 */
void ActivityPerson::selectNextActivity(){

    // No more to do, finish agent
    if( !this->selected_activity && this->to_do_list.isEmpty() && this->running_list.isEmpty() ){
        this->getTimer()->singleShot(0 , this , SLOT( agentEnded()) );
        return;
    }

    // If we have a parent activity as selected_activity, choose between its subactivities
    if( this->selected_activity && !this->selected_activity->getSubactivities().isEmpty() ){
        this->selected_activity = this->selectBestActivity( this->selected_activity->getSubactivities() );
    }

    // Else, choose from TODO list
    else {
        this->selected_activity = this->selectBestActivity( this->to_do_list );
    }

    if( !this->selected_activity ){
        qWarning() << "[ActivityPerson::selectNextActivity]" << TimeController::getCurrentDateTime().time() << "Agent has no activity to do now, retrying in 1 minute";
        this->getTimer()->singleShot( TimeController::secondsToEnvironmentMseconds(60) , this , SLOT( selectNextActivity()) );
        return;
    }

    // If the selected activity is in mandatory time, do not check preconditions
    if( this->selected_activity->isInMandatoryTime() ){
        this->getTimer()->singleShot( TimeController::secondsToEnvironmentMseconds(5) , this , SLOT( walkToSelectedActivity()) );
        return;
    }

    qDebug() << "[ActivityPerson::selectNextActivity]" << TimeController::getCurrentDateTime().time() << "Trying to start" << ActivityConstants::activityTypeToString( this->selected_activity->getType() );

    // Start flag
    bool start = true;

    // If the Activity has preconditions AND ARE NOT IN THE TODO LIST
    foreach ( ActivityConstants::activity_types precondition_type, this->selected_activity->getPreconditions() ){

        // Verify if precondition is already done
        Activity* precondition = this->getDoneActivity( precondition_type );

        // If precondition has not been done
        if( !precondition ){

            qDebug() << "[ActivityPerson::selectNextActivity] Missing precondition :" << ActivityConstants::activityTypeToString( precondition_type );

            // If the selected activity is a subactivity, check if the precondition is in its parents subactivities list
            if( this->selected_activity->getParentActivity() && !this->selected_activity->getParentActivity()->getSubactivities().isEmpty() ){
                QList<Activity*> subactivities = this->selected_activity->getParentActivity()->getSubactivities();
                for( int i = 0 ; i < subactivities.size() && !precondition ; i++ ){
                    if( subactivities.at(i)->getType() == precondition_type ){
                        precondition = subactivities.at(i);
                    }
                }
            }

            // Else verify if precondition is in the list of TODO activities
            if( !precondition ){
                precondition = this->getToDoActivity( precondition_type );
            }

            // Precondition is not in TODO LIST
            if( !precondition ){

                // Create the precondition activity
                precondition = ActivityCreator::createActivity( precondition_type, this );
                qDebug() << "[ActivityPerson::selectNextActivity] Creating new activity in TODO list :" << ActivityConstants::activityTypeToString( precondition->getType() );

                // Add to TODO list
                this->addToDoActivity( precondition );
            }

            // Make precondition as important as the selected activity
            precondition->setPriorityBase( this->selected_activity->calculateRouletteValue() + 1 );
            precondition->setPriorityIncrement( this->selected_activity->getPriorityIncrement() );

            start = false;
        }
    }

    // If the selected activity has mandatory time and it is not now, choose another one
    if( this->selected_activity->hasMandatoryTime() && !this->selected_activity->isInMandatoryTime() ){
        qDebug () << "[ActivityPerson::selectNextActivity] Selected activity has mandatory time and it is not now, selecting another activity";
        start = false;
    }

    // If the activity cannot be done because the appliance is in use
    if( this->selected_activity->applianceIsOccupied() ){
        qDebug () << "[ActivityPerson::selectNextActivity] Selected activity has appliance occupied";
        start = false;
    }

    // For some reason the selected activity cannot be started
    if( !start ){
        this->selected_activity->resetPriorityCounter();
        if( this->selected_activity->getParentActivity() ){
            this->selected_activity = this->selected_activity->getParentActivity();
        } else {
            this->selected_activity = 0;
        }
        this->getTimer()->singleShot( TimeController::secondsToEnvironmentMseconds(2) , this , SLOT( selectNextActivity()) );
        return;
    }

    // If they were no preconditions, continue with the chosen activity
    this->getTimer()->singleShot(0 , this , SLOT( walkToSelectedActivity()) );
}


void ActivityPerson::walkToSelectedActivity(){

    if( !this->selected_activity ){
        this->getTimer()->singleShot(0 , this , SLOT( selectNextActivity()) );
        return;
    }

    qDebug() << "[ActivityPerson::walkToSelectedActivity]" << TimeController::getCurrentDateTime().time() << "Walking to" << ActivityConstants::activityTypeToString( this->selected_activity->getType() );

    // Check if already there
    if( this->getGraphNode() == this->selected_activity->getGraphNode() ){
        this->getTimer()->singleShot(0 , this , SLOT( startSelectedActivity()) );
        return;
    }

    // Otherwise try to find a route
    GraphNode* to_node = this->selected_activity->getGraphNode();
    if( to_node && this->getGraphNode() ){
        DijkstraSkill* skill = new DijkstraSkill();
        skill->createGraph( Environment::getInstance()->getPhysicalGraph() );
        QList<GraphEdge*> route = skill->dijkstraShortestPath( this->getGraphNode() , to_node );
        skill->deleteLater();

        // If route found, get the first edge and move to it
        if( !route.isEmpty() ){
            GraphEdge* e = route.at(0);
            this->setGeometry( Environment::getInstance()->getGeometryFactory()->createPoint( e->getEndNode()->getCoordinate() ) );
            emit updateUISignal( AgentToUI::formatAgent( this , AgentToUI::UPDATE ) );
            this->getTimer()->singleShot( TimeController::secondsToEnvironmentMseconds( e->getCost() * 1000000 ) , this , SLOT( walkToSelectedActivity()) );
            return;
        }
    }

    // If cant route, start anyway
    if( this->selected_activity->getGraphNode() ){
        this->setGeometry( Environment::getInstance()->getGeometryFactory()->createPoint( this->selected_activity->getGraphNode()->getCoordinate() ) );
    }
    emit updateUISignal( AgentToUI::formatAgent( this , AgentToUI::UPDATE ) );
    this->getTimer()->singleShot(0 , this , SLOT( startSelectedActivity()) );
    return;
}


/**
 * @brief ActivityPerson::startSelectedActivity
 */
void ActivityPerson::startSelectedActivity(){

    if( !this->selected_activity ){
        this->getTimer()->singleShot(0 , this , SLOT( selectNextActivity()) );
        return;
    }

    // Remove started activity from TODO list (but not add to DONE list untill it has finished)
    this->to_do_list.removeAll( this->selected_activity ); // Remove it from TODO list, to not appear again while choosing among its subactivities

    qDebug() << "[ActivityPerson::startSelectedActivity]" << TimeController::getCurrentDateTime().time() << ActivityConstants::activityTypeToString( this->selected_activity->getType() ) << "started";

    // Disconnect if already connected to avoid repeated finish notifications
    this->disconnect( this->selected_activity , SIGNAL(activityEndedSignal( Activity* )) , this , SLOT(activityEnded( Activity* )) );
    // Connect to activity to know when it has finished
    this->connect( this->selected_activity , SIGNAL(activityEndedSignal( Activity* )) , this , SLOT(activityEnded( Activity* )) );

    // Set priority to base, not to be repeated again in a while
    this->selected_activity->resetPriorityCounter();

    // Add activity to RUNNING list
    this->running_list.append( this->selected_activity );

    // Start activity
    this->selected_activity->startActivity();

    // If started activity is unnatended, need to go to start another activity
    if( this->selected_activity->getAttendance() == ActivityConstants::UNATTENDED ){
        this->to_do_list.removeAll( this->selected_activity );

        // If it is a subactivity, set the selected activity to its parent
        if( this->selected_activity->getParentActivity() ){
            this->selected_activity = this->selected_activity->getParentActivity();
        }
        // Else, set it to 0 to choose from TODO list
        else {
            this->selected_activity = 0;
        }

        this->getTimer()->singleShot( TimeController::secondsToEnvironmentMseconds(5) , this , SLOT( selectNextActivity() ) );
        return;
    }

    // If the activity has no subactivities, it will finish automatically once the time has past
    // Otherwise, select from its subactivities and start them in order to finish this parent activity
    if( !this->selected_activity->getSubactivities().isEmpty() ){
        this->getTimer()->singleShot( TimeController::secondsToEnvironmentMseconds(5) , this , SLOT( selectNextActivity() ) );
        return;
    }
}


/**
 * @brief ActivityPerson::activityEnded
 */
void ActivityPerson::activityEnded( Activity* finished_activity ){

    this->disconnect( finished_activity , SIGNAL(activityEndedSignal( Activity* )) , this , SLOT(activityEnded( Activity* )) );

    qDebug() << "[ActivityPerson::activityEnded]" << TimeController::getCurrentDateTime().time() << ActivityConstants::activityTypeToString( finished_activity->getType() ) << "finished";    

    // Remove finished activity from RUNNING list
    this->running_list.removeAll( finished_activity );

    // Add finished activity to ready and DONE list
    this->done_list.append( finished_activity );

    // Add new activities that are consequence of the one that has just finished
    foreach (ActivityConstants::activity_types outcome, finished_activity->getOutcomes()){
        Activity* activity = this->getToDoActivity( outcome );
        if( activity ){
            // If activity was already in TODO list, duplicate its priority and duration
            activity->setPriorityBase( activity->getPriorityBase() * 2 );
            activity->setEstimatedDuration( activity->getEstimatedDuration() * 2 );
        } else {
            // Else create new activity and add it to TODO
            this->to_do_list.append( ActivityCreator::createActivity( outcome, this ) );
        }
    }

    // Remove the consumed activities from DONE list
    foreach (ActivityConstants::activity_types consume, finished_activity->getConsumes()){
        Activity* activity = this->getDoneActivity( consume );
        if( activity ){
            this->done_list.removeAll( activity );
        }
    }

    // Remove finished activity from to_do list
    this->to_do_list.removeAll( finished_activity );

    // If activity was unnatended, dont do nothing more as the agent will be doing some other activities
    if( finished_activity->getAttendance() == ActivityConstants::UNATTENDED ){
        return;
    }

    // See if the finished activity is a subactivity of a parent
    Activity* parent = finished_activity->getParentActivity();

    // If the finished is a subactivity and its parent is finished, this method will be called by it so do nothing with
    // the finished subactivity
    if( parent && parent->isFinished() ){
        return;
    }

    // If the parent is not finished, set it as selected activity and selectNextActivity will choose the next subactivity
    if( parent && !parent->isFinished() ){
        this->selected_activity = parent;
        this->getTimer()->singleShot( TimeController::secondsToEnvironmentMseconds(5) , this , SLOT( selectNextActivity()) );
        return;
    }

    // If the finished activity has no parent, go to choose the next activity from TODO list
    if( !parent ){
        this->selected_activity = 0;
        this->getTimer()->singleShot( TimeController::secondsToEnvironmentMseconds(5) , this , SLOT( selectNextActivity()) );
        return;
    }
}

/**
 * @brief ActivityPerson::agentEnded
 */
void ActivityPerson::agentEnded(){

    QString done_string = "";
    foreach (Activity* a, this->done_list) {
        done_string += ActivityConstants::activityTypeToString(a->getType()) + ", ";
    }

    qDebug() << "[ActivityPerson::agentStarted] FINISHING WITH DONE LIST" << done_string << this->done_list.size();

    if( this->selected_activity ){ this->selected_activity = 0; }
    Person::agentEnded();
}
