#ifndef ACTIVITYCONSTANTS_H
#define ACTIVITYCONSTANTS_H

#include <QObject>
#include <QDebug>
#include <QMap>
#include <QTime>

class ActivityConstants : public QObject
{
    Q_OBJECT

public:

    /**********************************************************************
     ACTIVITY ATTENDANCE
    **********************************************************************/

    enum attendance    {
        ATTENDED, // Person must only do and wait for this activity to finish
        SIMULTANEOUS, // Person can do only other activities which take place in the same location
        UNATTENDED // Person can do any other actiion/activity meanwhile
    };

    static attendance stringToAttendance(QString attendance){
        QString a = attendance.toLower();
        if ( a == "attended" ) { return ATTENDED; }
        if ( a == "simultaneous" ) { return SIMULTANEOUS; }
        if ( a == "unattended" ) { return UNATTENDED; }
        qWarning() << "[ActivityConstants::stringToActivityAttendance] Unknown activity attendance" << attendance << endl;
        return ATTENDED;
    }


    /**********************************************************************
     ACTIVITY TYPES
    **********************************************************************/

    enum activity_types {

        UNCLASSIFIED = -1,

        // BATHROOM
        USE_HAMPER,
        USE_SHOWER_FAUCET,
        USE_SINK_FAUCET_COLD,
        USE_SINK_FAUCET_HOT,
        USE_TOILET_FLUSH,
        BATHING,
        DRESSING,
        GROOMING,
        TOILETING,
        WASHING_HANDS,

        // KITCHEN
        USE_BURNER,
        USE_CABINET,
        USE_CEREAL,
        USE_COFFEE_MACHINE,
        USE_CONTAINER,

        USE_DISHWASHER,
        USE_DRAWER,
        USE_FAN,
        USE_FREEZER,
        USE_GARBAGE_DISPOSAL,
        USE_LAUNDRY_DRYER,
        USE_MEDICINE_CABINET,
        USE_MICROWAVE,

        USE_OVEN,
        USE_REFRIGERATOR,
        USE_SINK,
        USE_TOASTER,
        USE_WASHING_MACHINE,

        CLEANING,
        DOING_LAUNDRY,
        PREPARING_BEVERAGE,
        PREPARING_BREAKFAST,
        PREPARING_LUNCH,
        PREPARING_DINNER,
        PREPARING_SNACK,
        PUTTING_AWAY_DISHES,
        PUTTING_AWAY_LAUNDRY,
        PUTTING_AWAY_GROCERIES,
        TAKING_MEDICATION,
        WASHING_DISHES,

        // LIVING ROOM
        USE_CLOSET,
        USE_DVD,
        USE_JEWELRY_BOX,
        USE_LAMP,
        USE_STEREO,
        USE_TELEPHONE,
        USE_TV,
        HOME_EDUCATION,
        LISTENING_MUSIC,
        TALKING_ON_TELEPHONE,
        WATCHING_TV,
        WORKING_AT_COMPUTER,

        // FEYER
        LAWNWORK,
        GO_OUT_ENTERTAINING,
        GO_OUT_SHOPPING,
        GO_OUT_TO_WORK,

        // GENERAL
        USE_DOOR,
        USE_SWITCH,
        WAKE_UP
    };


    /**********************************************************************
     ACTIVITY DESCRIPTIONS
    **********************************************************************/

    struct ActivityDescription {
        ActivityConstants::activity_types activity_type;
        ActivityConstants::attendance attendance;
        double frequency;
        double priority_base;
        double priority_increment;
        double duration_mean;
        double duration_deviation;
        double finish_objective;
        double finish_increment;
        QTime desired_start_time;
        QTime desired_end_time;
        QTime mandatory_start_time;
        QTime mandatory_end_time;
        QList<ActivityConstants::activity_types> preconditions;
        QList<ActivityConstants::activity_types> outcomes;
        QList<ActivityConstants::activity_types> consumes;
        QString sensor_name;
    };

    /**********************************************************************
     STATIC METHODS
    **********************************************************************/

    static activity_types stringToActivityType(QString type){
        QString t = type.toUpper().trimmed();

        // BATHROOM
        if ( t == "USE_HAMPER" ) { return USE_HAMPER; }
        if ( t == "USE_SHOWER_FAUCET" ) { return USE_SHOWER_FAUCET; }
        if ( t == "USE_SINK_FAUCET_COLD" ) { return USE_SINK_FAUCET_COLD; }
        if ( t == "USE_SINK_FAUCET_HOT" ) { return USE_SINK_FAUCET_HOT; }
        if ( t == "USE_TOILET_FLUSH" ) { return USE_TOILET_FLUSH; }
        if ( t == "BATHING" ) { return BATHING; }
        if ( t == "DRESSING" ) { return DRESSING; }
        if ( t == "GROOMING" ) { return GROOMING; }
        if ( t == "TOILETING" ) { return TOILETING; }
        if ( t == "WASHING_HANDS" ) { return WASHING_HANDS; }

        // KITCHEN
        if ( t == "USE_BURNER" ) { return USE_BURNER; }
        if ( t == "USE_CABINET" ) { return USE_CABINET; }
        if ( t == "USE_CEREAL" ) { return USE_CEREAL; }
        if ( t == "USE_COFFEE_MACHINE" ) { return USE_COFFEE_MACHINE; }
        if ( t == "USE_CONTAINER" ) { return USE_CONTAINER; }
        if ( t == "USE_DRAWER" ) { return USE_DRAWER; }
        if ( t == "USE_DISHWASHER" ) { return USE_DRAWER; }
        if ( t == "USE_FAN" ) { return USE_FAN; }
        if ( t == "USE_FREEZER" ) { return USE_FREEZER; }
        if ( t == "USE_GARBAGE_DISPOSAL" ) { return USE_GARBAGE_DISPOSAL; }
        if ( t == "USE_LAUNDRY_DRYER" ) { return USE_LAUNDRY_DRYER; }
        if ( t == "USE_MEDICINE_CABINET" ) { return USE_MEDICINE_CABINET; }
        if ( t == "USE_MICROWAVE" ) { return USE_MICROWAVE; }
        if ( t == "USE_OVEN" ) { return USE_OVEN; }
        if ( t == "USE_REFRIGERATOR" ) { return USE_REFRIGERATOR; }
        if ( t == "USE_SINK" ) { return USE_SINK; }
        if ( t == "USE_TOASTER" ) { return USE_TOASTER; }
        if ( t == "USE_WASHING_MACHINE" ) { return USE_WASHING_MACHINE; }
        if ( t == "CLEANING" ) { return CLEANING; }
        if ( t == "DOING_LAUNDRY" ) { return DOING_LAUNDRY; }
        if ( t == "PREPARING_BEVERAGE" ) { return PREPARING_BEVERAGE; }
        if ( t == "PREPARING_BREAKFAST" ) { return PREPARING_BREAKFAST; }
        if ( t == "PREPARING_LUNCH" ) { return PREPARING_LUNCH; }
        if ( t == "PREPARING_DINNER" ) { return PREPARING_DINNER; }
        if ( t == "PREPARING_SNACK" ) { return PREPARING_SNACK; }
        if ( t == "PUTTING_AWAY_DISHES" ) { return PUTTING_AWAY_DISHES; }
        if ( t == "PUTTING_AWAY_LAUNDRY" ) { return PUTTING_AWAY_LAUNDRY; }
        if ( t == "PUTTING_AWAY_GROCERIES" ) { return PUTTING_AWAY_GROCERIES; }
        if ( t == "TAKING_MEDICATION" ) { return TAKING_MEDICATION; }
        if ( t == "WASHING_DISHES" ) { return WASHING_DISHES; }

        // LIVING ROOM        
        if ( t == "USE_CLOSET" ) { return USE_CLOSET; }
        if ( t == "USE_DVD" ) { return USE_DVD; }
        if ( t == "USE_JEWELRY_BOX" ) { return USE_JEWELRY_BOX; }
        if ( t == "USE_LAMP" ) { return USE_LAMP; }
        if ( t == "USE_STEREO" ) { return USE_STEREO; }
        if ( t == "USE_TELEPHONE" ) { return USE_TELEPHONE; }
        if ( t == "USE_TV" ) { return USE_TV; }
        if ( t == "HOME_EDUCATION" ) { return HOME_EDUCATION; }
        if ( t == "LISTENING_MUSIC" ) { return LISTENING_MUSIC; }
        if ( t == "TALKING_ON_TELEPHONE" ) { return TALKING_ON_TELEPHONE; }
        if ( t == "WATCHING_TV" ) { return WATCHING_TV; }
        if ( t == "WORKING_AT_COMPUTER" ) { return WORKING_AT_COMPUTER; }

        // FEYER     
        if ( t == "LAWNWORK" ) { return LAWNWORK; }
        if ( t == "GO_OUT_ENTERTAINING" ) { return GO_OUT_ENTERTAINING; }
        if ( t == "GO_OUT_SHOPPING" ) { return GO_OUT_SHOPPING; }
        if ( t == "GO_OUT_TO_WORK" ) { return GO_OUT_TO_WORK; }

        // GENERAL
        if ( t == "USE_DOOR" ) { return USE_DOOR; }
        if ( t == "USE_SWITCH" ) { return USE_SWITCH; }
        if ( t == "WAKE_UP" ) { return WAKE_UP; }

        qWarning() << "[ActivityConstants::stringToActivityType] Unknown activity type" << type << endl;
        return UNCLASSIFIED;
    }

    static QString activityTypeToString(activity_types type){

        // BATHROOM
        if ( type == USE_HAMPER ) { return "USE_HAMPER"; }
        if ( type == USE_SHOWER_FAUCET ) { return "USE_SHOWER_FAUCET"; }
        if ( type == USE_SINK_FAUCET_COLD ) { return "USE_SINK_FAUCET_COLD"; }
        if ( type == USE_SINK_FAUCET_HOT ) { return "USE_SINK_FAUCET_HOT"; }
        if ( type == USE_TOILET_FLUSH ) { return "USE_TOILET_FLUSH"; }
        if ( type == BATHING ) { return "BATHING"; }
        if ( type == DRESSING ) { return "DRESSING"; }
        if ( type == GROOMING ) { return "GROOMING"; }
        if ( type == TOILETING ) { return "TOILETING"; }
        if ( type == WASHING_HANDS ) { return "WASHING_HANDS"; }

        // KITCHEN
        if ( type == USE_BURNER ) { return "USE_BURNER"; }
        if ( type == USE_CABINET) { return "USE_CABINET"; }
        if ( type == USE_CEREAL) { return "USE_CEREAL"; }
        if ( type == USE_COFFEE_MACHINE) { return "USE_COFFEE_MACHINE"; }
        if ( type == USE_CONTAINER) { return "USE_CONTAINER"; }
        if ( type == USE_DRAWER) { return "USE_DRAWER"; }
        if ( type == USE_DISHWASHER) { return "USE_DISHWASHER"; }
        if ( type == USE_FAN) { return "USE_FAN"; }
        if ( type == USE_FREEZER) { return "USE_FREEZER"; }
        if ( type == USE_GARBAGE_DISPOSAL ) { return "USE_GARBAGE_DISPOSAL"; }
        if ( type == USE_LAUNDRY_DRYER ) { return "USE_LAUNDRY_DRYER"; }
        if ( type == USE_MEDICINE_CABINET) { return "USE_MEDICINE_CABINET"; }
        if ( type == USE_MICROWAVE ) { return "USE_MICROWAVE"; }
        if ( type == USE_OVEN ) { return "USE_OVEN"; }
        if ( type == USE_REFRIGERATOR ) { return "USE_REFRIGERATOR"; }
        if ( type == USE_SINK ) { return "USE_SINK"; }
        if ( type == USE_TOASTER ) { return "USE_TOASTER"; }
        if ( type == USE_WASHING_MACHINE ) { return "USE_WASHING_MACHINE"; }
        if ( type == CLEANING) { return "CLEANING"; }
        if ( type == DOING_LAUNDRY) { return "DOING_LAUNDRY"; }
        if ( type == PREPARING_BEVERAGE ) { return "PREPARING_BEVERAGE"; }
        if ( type == PREPARING_BREAKFAST ) { return "PREPARING_BREAKFAST"; }
        if ( type == PREPARING_LUNCH ) { return "PREPARING_LUNCH"; }
        if ( type == PREPARING_DINNER ) { return "PREPARING_DINNER"; }
        if ( type == PREPARING_SNACK ) { return "PREPARING_SNACK"; }
        if ( type == PUTTING_AWAY_DISHES) { return "PUTTING_AWAY_DISHES"; }
        if ( type == PUTTING_AWAY_LAUNDRY) { return "PUTTING_AWAY_LAUNDRY"; }
        if ( type == PUTTING_AWAY_GROCERIES) { return "PUTTING_AWAY_GROCERIES"; }
        if ( type == TAKING_MEDICATION) { return "TAKING_MEDICATION"; }
        if ( type == WASHING_DISHES) { return "WASHING_DISHES"; }

        // LIVING ROOM
        if ( type == USE_CLOSET) { return "USE_CLOSET"; }
        if ( type == USE_DVD) { return "USE_DVD"; }
        if ( type == USE_JEWELRY_BOX) { return "USE_JEWELRY_BOX"; }
        if ( type == USE_LAMP) { return "USE_LAMP"; }
        if ( type == USE_STEREO) { return "USE_STEREO"; }
        if ( type == USE_TELEPHONE) { return "USE_TELEPHONE"; }
        if ( type == USE_TV) { return "USE_TV"; }
        if ( type == HOME_EDUCATION) { return "HOME_EDUCATION"; }
        if ( type == LISTENING_MUSIC) { return "LISTENING_MUSIC"; }
        if ( type == TALKING_ON_TELEPHONE) { return "TALKING_ON_TELEPHONE"; }
        if ( type == WATCHING_TV) { return "WATCHING_TV"; }
        if ( type == WORKING_AT_COMPUTER) { return "WORKING_AT_COMPUTER"; }

        // FEYER
        if ( type == LAWNWORK) { return "LAWNWORK"; }
        if ( type == GO_OUT_ENTERTAINING) { return "GO_OUT_ENTERTAINING"; }
        if ( type == GO_OUT_SHOPPING) { return "GO_OUT_SHOPPING"; }
        if ( type == GO_OUT_TO_WORK) { return "GO_OUT_TO_WORK"; }

        // GENERAL
        if ( type == USE_DOOR) { return "USE_DOOR"; }
        if ( type == USE_SWITCH) { return "USE_SWITCH"; }
        if ( type == WAKE_UP) { return "WAKE_UP"; }

        qWarning() << "[ActivityConstants::activityTypeToString] Unknown activity type" << type << endl;
        return "UNCLASSIFIED";
    }

};

#endif // ACTIVITYCONSTANTS_H

