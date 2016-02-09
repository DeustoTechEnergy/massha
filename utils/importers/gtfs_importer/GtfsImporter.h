#ifndef GTFSIMPORTER_H
#define GTFSIMPORTER_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QTime>

#include "agents/roads/PublicTransportStop.h"
#include "agents/people/vehicle_drivers/ItineraryVehicleDriver.h"

// HELPER CLASSES
// https://developers.google.com/transit/gtfs/reference
struct Agency {
    QString     agency_id; // Is key
    QString     agency_name;
    QString     agency_url;
    QString     agency_timezone;
    QString     agency_lang;
    QString     agency_phone;
    QString     agency_fare_url;
};

struct Stop {
    QString     stop_id; // Is key
    QString     stop_code;
    QString     stop_name;
    QString     stop_desc;
    double      stop_lat;
    double      stop_lon;
    int         zone_id;
    QString     stop_url;
    int         location_type;
    QString     parent_station;
    QString     stop_timezone;
    QString     wheelchair_boarding;
};

struct Route {
    QString     route_id; // Is key
    QString     agency_id;
    QString     route_short_name;
    QString     route_long_name;
    QString     route_desc;
    int         route_type;
    QString     route_url;
    QString     route_color;
    QString     route_text_color;
};

struct Trip {
    QString     route_id;
    QString     service_id;
    QString     trip_id; // Is key
    QString     trip_headsign;
    QString     trip_short_name;
    int         direction_id;
    QString     block_id;
    QString     shape_id;
    int         wheelchair_accessible;
    int         bikes_allowed;
};

struct StopTime {
    QString     trip_id; // Is key for grouping them
    QTime       arrival_time;
    QTime       departure_time;
    QString     stop_id;
    int         stop_sequence;
    QString     stop_headsign;
    int         pickup_type;
    int         drop_off_type;
    double      shape_dist_traveled;
    int         timepoint;
};

struct Calendar {
    QString     service_id; // Is key
    bool        monday;
    bool        tuesday;
    bool        wednesday;
    bool        thursday;
    bool        friday;
    bool        saturday;
    bool        sunday;
    QDate       start_date;
    QDate       end_date;
};

struct CalendarDate {
    QString     service_id; // Is key
    QDate       date;
    int         exception_type;
};

struct Shape {
    QString     shape_id; // Is key for grouping them
    double      shape_pt_lat;
    double      shape_pt_lon;
    int         shape_pt_sequence;
    double      shape_dist_traveled;
};



class GtfsImporter : public QObject
{
    Q_OBJECT
public:
    explicit GtfsImporter(QString path);
    ~GtfsImporter();

    // GETTERS
    QList<Agency> getAgencies();
    Agency getAgencyById(QString id);

    QList<Stop> getStops();
    Stop getStopById(QString id);

    QList<Route> getRoutes();
    Route getRouteById(QString id);

    QList<Trip> getTrips();
    Trip getTripById(QString id);

    QList<StopTime> getStopTimes();
    QList<StopTime> getStopTimesByTripId(QString id);

    QList<Calendar> getCalendars();
    Calendar getCalendarByServiceId(QString id);

    QList<CalendarDate> getCalendarDates();
    QList<CalendarDate> getCalendarDatesByServiceId(QString id);

    QList<Shape> getShapes();
    QList<Shape> getShapesByShapeId(QString id);

    // METHODS
    QList<ItineraryVehicleDriver*> toDrivers( QList<PublicTransportStop*> snap_stops );

private:

    // MAPS
    QMap<QString , Agency> agencies;
    QMap<QString , Stop> stops;
    QMap<QString , Route> routes;
    QMap<QString , Trip> trips;
    QMap<QString , QList<StopTime> > stoptimes;
    QMap<QString , Calendar> calendars;
    QMap<QString , QList<CalendarDate> > calendardates;
    QMap<QString , QList<Shape> > shapes;

};

#endif // GTFSIMPORTER_H
