#ifndef ROADCONSTANTS_H
#define ROADCONSTANTS_H

#include <QObject>
#include <QDebug>

class RoadConstants : public QObject
{
    Q_OBJECT
    Q_ENUMS(road_types)
public:

    enum road_types         {
        // HIGHWAY
        MOTORWAY = 101,
        MOTORWAY_LINK = 102,
        MOTORWAY_JUNCTION = 103,
        TRUNK = 104,
        TRUNK_LINK = 105,
        PRIMARY = 106,
        PRIMARY_LINK = 107,
        SECONDARY = 108,
        SECONDARY_LINK = 109,
        TERTIARY = 110,
        TERTIARY_LINK = 111,
        RESIDENTIAL = 112,
        LIVING_STREET = 113,
        SERVICE = 114,
        TRACK = 115,
        PATH = 116,
        CYCLEWAY = 117,
        BRIDLEWAY = 118,
        PEDESTRIAN = 119,
        FOOTWAY = 120,
        STEPS = 121,
        UNCLASSIFIED = 198,
        ROAD = 199,
        // JUNCTION
        ROUNDABOUT = 401,
        // CYCLEWAY
        CYCLE_LANE = 201,
        CYCLE_TRACK = 202,
        CYCLE_OPPOSITE_TRACK = 203,
        CYCLE_OPPOSITE = 204,
        // RAILWAY
        FUNICULAR = 501,
        LIGHT_RAIL = 502,
        NARROW_GAUGE = 503,
        PRESERVED = 504,
        RAIL = 505,
        SUBWAY = 506,
        TRAM = 507,
    };

    static RoadConstants::road_types osmKeyValueToRoadType(QString k, QString v){
        QString key = k.toLower().trimmed();
        QString value = v.toLower().trimmed();
        if( key == "highway"){
            if( value == "motorway") return RoadConstants::MOTORWAY;
            if( value == "motorway_link") return RoadConstants::MOTORWAY_LINK;
            if( value == "motorway_junction") return RoadConstants::MOTORWAY_JUNCTION;
            if( value == "trunk") return RoadConstants::TRUNK;
            if( value == "trunk_link") return RoadConstants::TRUNK_LINK;
            if( value == "primary") return RoadConstants::PRIMARY;
            if( value == "primary_link") return RoadConstants::PRIMARY_LINK;
            if( value == "secondary") return RoadConstants::SECONDARY;
            if( value == "secondary_link") return RoadConstants::SECONDARY_LINK;
            if( value == "tertiary") return RoadConstants::TERTIARY;
            if( value == "tertiary_link") return RoadConstants::TERTIARY_LINK;
            if( value == "residential") return RoadConstants::RESIDENTIAL;
            if( value == "road") return RoadConstants::ROAD;
            if( value == "living_street") return RoadConstants::LIVING_STREET;
            if( value == "service") return RoadConstants::SERVICE;
            if( value == "track") return RoadConstants::TRACK;
            if( value == "path") return RoadConstants::PATH;
            if( value == "unclassified") return RoadConstants::UNCLASSIFIED;
            if( value == "pedestrian") return RoadConstants::PEDESTRIAN;
            if( value == "footway") return RoadConstants::FOOTWAY;
            if( value == "steps") return RoadConstants::STEPS;
            if( value == "cycleway") return RoadConstants::CYCLEWAY;

        } if( key == "junction"){
            if( value == "roundabout") return RoadConstants::ROUNDABOUT;

        } if( key == "cycleway"){
            if( value == "cycleway") return RoadConstants::CYCLEWAY;
            if( value == "cycle_lane") return RoadConstants::CYCLE_LANE;
            if( value == "cycle_track") return RoadConstants::CYCLE_TRACK;
            if( value == "track") return RoadConstants::CYCLE_TRACK;
            if( value == "cycle_opposite_track") return RoadConstants::CYCLE_OPPOSITE_TRACK;
            if( value == "cycle_opposite") return RoadConstants::CYCLE_OPPOSITE;
            if( value == "bridleway") return RoadConstants::BRIDLEWAY;

        } if( key == "railway"){
            if( value == "funicular") return RoadConstants::FUNICULAR;
            if( value == "light_rail") return RoadConstants::LIGHT_RAIL;
            if( value == "narrow_gauge") return RoadConstants::NARROW_GAUGE;
            if( value == "tram") return RoadConstants::TRAM;
            if( value == "subway") return RoadConstants::SUBWAY;
            if( value == "preserved") return RoadConstants::PRESERVED;
            if( value == "rail") return RoadConstants::RAIL;

        }

        qWarning() << "[RoadConstants::osmKeyValueToRoadType] Road key-value not matched key:" << key << "value:" << value;
        return RoadConstants::UNCLASSIFIED;
    }

    static QString roadTypeToString(RoadConstants::road_types type){
        switch (type){
        case RoadConstants::MOTORWAY: return "motorway";
        case RoadConstants::MOTORWAY_LINK: return "motorway_link";
        case RoadConstants::MOTORWAY_JUNCTION: return "motorway_junction";
        case RoadConstants::TRUNK: return "trunk";
        case RoadConstants::TRUNK_LINK: return "trunk_link";
        case RoadConstants::PRIMARY: return "primary";
        case RoadConstants::PRIMARY_LINK: return "primary_link";
        case RoadConstants::SECONDARY: return "secondary";
        case RoadConstants::SECONDARY_LINK: return "secondary_link";
        case RoadConstants::TERTIARY: return "tertiary";
        case RoadConstants::TERTIARY_LINK: return "tertiary_link";
        case RoadConstants::ROUNDABOUT: return "roundabout";
        case RoadConstants::RESIDENTIAL: return "residential";
        case RoadConstants::LIVING_STREET: return "living_street";
        case RoadConstants::SERVICE: return "service";
        case RoadConstants::TRACK: return "track";
        case RoadConstants::PATH: return "path";
        case RoadConstants::CYCLEWAY: return "cycleway";
        case RoadConstants::BRIDLEWAY: return "bridleway";
        case RoadConstants::PEDESTRIAN: return "pedestrian";
        case RoadConstants::FOOTWAY: return "footway";
        case RoadConstants::STEPS: return "steps";
        case RoadConstants::UNCLASSIFIED: return "unclassified";
        case RoadConstants::ROAD: return "road";
        case RoadConstants::CYCLE_LANE: return "cycle_lane";
        case RoadConstants::CYCLE_TRACK: return "cycle_track";
        case RoadConstants::CYCLE_OPPOSITE_TRACK: return "cycle_opposite_track";
        case RoadConstants::CYCLE_OPPOSITE: return "cycle_opposite";
        case RoadConstants::FUNICULAR: return "funicular";
        case RoadConstants::LIGHT_RAIL: return "light_rail";
        case RoadConstants::NARROW_GAUGE: return "narrow_gauge";
        case RoadConstants::PRESERVED: return "preserved";
        case RoadConstants::RAIL: return "rail";
        case RoadConstants::SUBWAY: return "subway";
        case RoadConstants::TRAM: return "tram";
        }
        return "";
    }

    static double roadTypeDefaultSpeed(RoadConstants::road_types type){
        switch (type){
        case RoadConstants::MOTORWAY:
            return 33.3;
        case RoadConstants::MOTORWAY_LINK:
        case RoadConstants::MOTORWAY_JUNCTION:
        case RoadConstants::TRUNK:
            return 27.7;
        case RoadConstants::TRUNK_LINK:
        case RoadConstants::PRIMARY:
            return 25;
        case RoadConstants::PRIMARY_LINK:
        case RoadConstants::SECONDARY:
            return 22.2;
        case RoadConstants::SECONDARY_LINK:
        case RoadConstants::TERTIARY:
        case RoadConstants::ROUNDABOUT:
            return 19.4;
        case RoadConstants::TERTIARY_LINK:
        case RoadConstants::RESIDENTIAL:
        case RoadConstants::ROAD:
            return 13.9;
        case RoadConstants::LIVING_STREET:
        case RoadConstants::SERVICE:
            return 5.55;
        case RoadConstants::TRACK:
        case RoadConstants::PATH:
        case RoadConstants::UNCLASSIFIED:
            return 1;
        case RoadConstants::CYCLEWAY:
        case RoadConstants::CYCLE_LANE:
        case RoadConstants::CYCLE_TRACK:
        case RoadConstants::CYCLE_OPPOSITE_TRACK:
        case RoadConstants::CYCLE_OPPOSITE:
        case RoadConstants::BRIDLEWAY:
            return 11.1;
        case RoadConstants::PEDESTRIAN:
        case RoadConstants::FOOTWAY:
        case RoadConstants::STEPS:
            return 3;

        case RoadConstants::FUNICULAR:
            return 13.9;

        case RoadConstants::LIGHT_RAIL:
        case RoadConstants::NARROW_GAUGE:
        case RoadConstants::TRAM:
        case RoadConstants::SUBWAY:
            return 27.7;
        case RoadConstants::PRESERVED:
            return 19.4;
        case RoadConstants::RAIL:
            return 44.4;
        }
        return 1;
    }

};


#endif // ROADCONSTANTS_H

