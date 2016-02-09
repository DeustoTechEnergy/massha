#ifndef ACTIVITYCREATOR
#define ACTIVITYCREATOR

#include "activities/ActivityConstants.h"
#include "activities/Activity.h"
#include "activities/SwitchableApplianceActivity.h"
#include "activities/TimeCountdownActivity.h"
#include "skills/random_generators/NormalDistributionSkill.h"

class ActivityCreator : public QObject
{
    Q_OBJECT

public:

    /**********************************************************************
     STORE A DESCRIPTION
    **********************************************************************/

    static ActivityConstants::ActivityDescription createActivityDescription( QJsonObject json ){
        ActivityConstants::ActivityDescription desc;
        desc.activity_type = ActivityConstants::stringToActivityType( json["activity_type"].toString( "ERROR" ) );
        //subactivity.sensor_name = json_subactivity["sensor_name"].toString( "No name" );
        desc.attendance = ActivityConstants::stringToAttendance( json["attendance"].toString( "ATTENDED" ) );
        desc.frequency = json["frequency"].toDouble( 0 );
        desc.duration_mean = json["duration_mean"].toDouble( 0 );
        desc.duration_deviation= json["duration_deviation"].toDouble( 0 );
        desc.finish_objective = json["finish_objective"].toDouble( 0 );
        desc.finish_increment = json["finish_increment"].toDouble( 0.1 );
        desc.frequency = json["frequency"].toDouble( 1 );
        desc.priority_base = json["priority_base"].toDouble( 0 );
        desc.priority_increment = json["priority_increment"].toDouble( 0 );
        desc.desired_start_time = QTime::fromString( json["desired_start_time"].toString( "Invalid" ) , "hh:mm" );
        desc.desired_end_time = QTime::fromString( json["desired_end_time"].toString( "Invalid" ) , "hh:mm" );
        desc.mandatory_start_time = QTime::fromString( json["mandatory_start_time"].toString( "Invalid" ) , "hh:mm" );
        desc.mandatory_end_time = QTime::fromString( json["mandatory_end_time"].toString( "Invalid" ) , "hh:mm" );
        desc.sensor_name = json["sensor_name"].toString( "" );

        if ( json["preconditions"].isArray() ){
            foreach ( QJsonValue p, json["preconditions"].toArray() ){
                desc.preconditions.append( ActivityConstants::stringToActivityType( p.toString() ));
            }
        }
        if ( json["outcomes"].isArray() ){
            foreach ( QJsonValue p, json["outcomes"].toArray() ){
                desc.outcomes.append( ActivityConstants::stringToActivityType( p.toString() ));
            }
        }
        if ( json["consumes"].isArray() ){
            foreach ( QJsonValue p, json["consumes"].toArray() ){
                desc.consumes.append( ActivityConstants::stringToActivityType( p.toString() ));
            }
        }

        return desc;
    }

    /**********************************************************************
     LOAD DEFAULT VALUES FROM DESCRIPTION TO ACTIVITY
    **********************************************************************/

    static Activity* loadActivityFromDescription( Activity* activity , ActivityConstants::ActivityDescription description ){
        NormalDistributionSkill* nds = new NormalDistributionSkill();
        activity->setAttendance( description.attendance );
        activity->setPriorityBase( description.priority_base );
        activity->setPriorityIncrement( description.priority_increment );
        activity->setFinishObjective( description.finish_objective );
        activity->setFinishIncrement( description.finish_increment );
        activity->setFrequency( description.frequency );
        activity->setDesiredStartTime( description.desired_start_time );
        activity->setDesiredEndTime( description.desired_end_time );
        activity->setMandatoryStartTime( description.mandatory_start_time );
        activity->setMandatoryEndTime( description.mandatory_end_time );
        activity->setEstimatedDuration( nds->normalDistribution( description.duration_mean, description.duration_mean * 0.10 ) );
        activity->setSensorName( description.sensor_name );
        foreach(ActivityConstants::activity_types t , description.preconditions ){ activity->addPrecondition(t); }
        foreach(ActivityConstants::activity_types t , description.outcomes ){ activity->addOutcome(t); }
        foreach(ActivityConstants::activity_types t , description.consumes ){ activity->addConsume(t); }
        nds->deleteLater();
        return activity;
    }

    /**********************************************************************
     ACTIVITY CREATOR
    **********************************************************************/

    static Activity* createActivity(ActivityConstants::activity_types type, Agent* activity_doer){
        Activity* a = 0;

        switch (type) {

        // BATHROOM
        case ActivityConstants::USE_HAMPER :            a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::USE_SHOWER_FAUCET :     a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::USE_SINK_FAUCET_COLD :  a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::USE_SINK_FAUCET_HOT :   a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::USE_TOILET_FLUSH :      a = new TimeCountdownActivity( type ); break;
        case ActivityConstants::BATHING :               a = new Activity( type ); break;
        case ActivityConstants::DRESSING:               a = new Activity( type ); break;
        case ActivityConstants::GROOMING:               a = new Activity( type ); break;
        case ActivityConstants::TOILETING :             a = new Activity( type ); break;
        case ActivityConstants::WASHING_HANDS:          a = new Activity( type ); break;

        // KITCHEN
        case ActivityConstants::USE_BURNER:             a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::USE_CABINET :           a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::USE_CEREAL :            a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::USE_COFFEE_MACHINE :    a = new TimeCountdownActivity( type ); break;
        case ActivityConstants::USE_CONTAINER :         a = new SwitchableApplianceActivity( type ); break;

        case ActivityConstants::USE_DISHWASHER :        a = new TimeCountdownActivity( type ); break;
        case ActivityConstants::USE_DRAWER :            a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::USE_FAN :               a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::USE_FREEZER :           a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::USE_GARBAGE_DISPOSAL :  a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::USE_LAUNDRY_DRYER :     a = new TimeCountdownActivity( type ); break;
        case ActivityConstants::USE_MEDICINE_CABINET :  a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::USE_MICROWAVE :         a = new TimeCountdownActivity( type ); break;

        case ActivityConstants::USE_OVEN :              a = new TimeCountdownActivity( type ); break;
        case ActivityConstants::USE_REFRIGERATOR :      a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::USE_SINK :              a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::USE_TOASTER :           a = new TimeCountdownActivity( type ); break;
        case ActivityConstants::USE_WASHING_MACHINE :   a = new TimeCountdownActivity( type ); break;

        case ActivityConstants::CLEANING :              a = new Activity( type ); break;
        case ActivityConstants::DOING_LAUNDRY :         a = new Activity( type ); break;
        case ActivityConstants::PREPARING_BEVERAGE :    a = new Activity( type ); break;
        case ActivityConstants::PREPARING_BREAKFAST :   a = new Activity( type ); break;
        case ActivityConstants::PREPARING_LUNCH :       a = new Activity( type ); break;
        case ActivityConstants::PREPARING_DINNER :      a = new Activity( type ); break;
        case ActivityConstants::PREPARING_SNACK :       a = new Activity( type ); break;
        case ActivityConstants::PUTTING_AWAY_DISHES :   a = new Activity( type ); break;
        case ActivityConstants::PUTTING_AWAY_LAUNDRY :  a = new Activity( type ); break;
        case ActivityConstants::PUTTING_AWAY_GROCERIES: a = new Activity( type ); break;
        case ActivityConstants::TAKING_MEDICATION :     a = new Activity( type ); break;
        case ActivityConstants::WASHING_DISHES :        a = new Activity( type ); break;


        // LIVING ROOM
        case ActivityConstants::USE_CLOSET :            a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::USE_DVD :               a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::USE_JEWELRY_BOX :       a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::USE_LAMP :              a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::USE_STEREO :            a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::USE_TELEPHONE :         a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::USE_TV :                a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::HOME_EDUCATION :        a = new Activity( type ); break;
        case ActivityConstants::LISTENING_MUSIC:        a = new Activity( type ); break;
        case ActivityConstants::TALKING_ON_TELEPHONE :  a = new Activity( type ); break;
        case ActivityConstants::WATCHING_TV :           a = new Activity( type ); break;
        case ActivityConstants::WORKING_AT_COMPUTER :   a = new Activity( type ); break;

        // FEYER
        case ActivityConstants::LAWNWORK :              a = new Activity( type ); break;
        case ActivityConstants::GO_OUT_ENTERTAINING :   a = new Activity( type ); break;
        case ActivityConstants::GO_OUT_SHOPPING :       a = new Activity( type ); break;
        case ActivityConstants::GO_OUT_TO_WORK :        a = new Activity( type ); break;

        // GENERAL
        case ActivityConstants::USE_DOOR :              a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::USE_SWITCH :            a = new SwitchableApplianceActivity( type ); break;
        case ActivityConstants::WAKE_UP :               a = new Activity( type ); break;

        default: a = 0;
        }

        if( a ){

            a = ActivityCreator::loadActivityFromDescription( a , ActivityCreator::activity_descriptions.value( a->getType() ) );
            a->setActivityDoer( activity_doer );

            foreach(ActivityConstants::ActivityDescription d , ActivityCreator::subactivity_descriptions.value( a->getType() ) ){
                Activity* sub = ActivityCreator::createActivity( d.activity_type , activity_doer );

                if ( !sub ){
                    continue;
                }

                sub = ActivityCreator::loadActivityFromDescription( sub , d );
                sub->setActivityDoer( activity_doer );

                // Set parent to attended
                a->setAttendance( ActivityConstants::ATTENDED );
                a->addSubactivity( sub );
            }
        }
        return a;
    }

    static void addActivityDescription( ActivityConstants::ActivityDescription activity ){
        if ( !ActivityCreator::activity_descriptions.contains( activity.activity_type ) ){
            ActivityCreator::activity_descriptions.insert(activity.activity_type, activity );
        } else {
            ActivityCreator::activity_descriptions[activity.activity_type] = activity;
        }
    }

    static void addSubactivityDescription( ActivityConstants::activity_types activity_type, ActivityConstants::ActivityDescription subactivity ){
        if ( !ActivityCreator::subactivity_descriptions.contains( activity_type ) ){
            QList< ActivityConstants::ActivityDescription > subactivities;
            ActivityCreator::subactivity_descriptions.insert( activity_type, subactivities );
        }
        QList<ActivityConstants::ActivityDescription> subactivities = ActivityCreator::subactivity_descriptions.value( activity_type );
        subactivities.append( subactivity );
        ActivityCreator::subactivity_descriptions[ activity_type ] = subactivities;
    }

    static QMap<ActivityConstants::activity_types, ActivityConstants::ActivityDescription > activity_descriptions;
    static QMap<ActivityConstants::activity_types, QList<ActivityConstants::ActivityDescription> > subactivity_descriptions;

};

#endif // ACTIVITYCREATOR

