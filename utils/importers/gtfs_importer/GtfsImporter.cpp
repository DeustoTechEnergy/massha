#include "GtfsImporter.h"

#include <QDir>
#include <QFile>
#include <QVector>
#include <QStringList>
#include <QDebug>

#include "environment/Environment.h"

/**
 * @brief GtfsImporter Give the name of the folder containing the GTFS files.
 * All files must have the standard name.
 * @param path
 * @return
 */
GtfsImporter::GtfsImporter(QString path){

    QDir dir(path);
    if( !dir.exists() ){
        qWarning() << "[GtfsImporter::GtfsImporter] Path doesnt exist";
        return;
    }

    // Load Agency.txt
    {
        QStringList ss;
        ss << "agency_id" << "agency_name" << "agency_phone" << "agency_url" << "agency_timezone" << "agency_lang";
        QFile file( dir.absoluteFilePath("agency.txt") );
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QVector<QString>* positions = 0;
            QTextStream in(&file);
            while ( !in.atEnd() ) {
                QString line = in.readLine();
                QStringList values = line.split(',');
                if( !positions ){
                    positions = new QVector<QString>( values.size() );
                    foreach(QString s , ss){
                        if( values.contains(s) ){
                            positions->replace( values.indexOf(s) , s);
                        }
                    }
                } else {
                    Agency a;
                    if( positions->contains("agency_id") ){         a.agency_id = values.at( positions->indexOf("agency_id") ); }
                    if( positions->contains("agency_name") ){       a.agency_name = values.at( positions->indexOf("agency_name") ); }
                    if( positions->contains("agency_phone") ){      a.agency_phone = values.at( positions->indexOf("agency_phone") ); }
                    if( positions->contains("agency_url") ){        a.agency_url = values.at( positions->indexOf("agency_url") ); }
                    if( positions->contains("agency_timezone") ){   a.agency_timezone = values.at( positions->indexOf("agency_timezone") ); }
                    if( positions->contains("agency_lang") ){       a.agency_lang = values.at( positions->indexOf("agency_lang") ); }
                    this->agencies.insert( a.agency_id , a );
                }
            }
            delete positions;
        }
        file.close();
    }

    // Load Stops.txt
    {
        QStringList ss;
        ss << "stop_id" << "stop_code" << "stop_name" << "stop_desc" << "stop_lat" << "stop_lon" << "zone_id" << "stop_url" << "location_type" << "parent_station" << "stop_timezone" << "wheelchair_boarding";
        QFile file( dir.absoluteFilePath("stops.txt") );
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QVector<QString>* positions = 0;
            QTextStream in(&file);
            while ( !in.atEnd() ) {
                QString line = in.readLine();
                QStringList values = line.split(',');
                if( !positions ){
                    positions = new QVector<QString>( values.size() );
                    foreach(QString s , ss){
                        if( values.contains(s) ){
                            positions->replace( values.indexOf(s) , s);
                        }
                    }
                } else {
                    Stop s;
                    if( positions->contains("stop_id") ){          s.stop_id = values.at( positions->indexOf("stop_id") ); }
                    if( positions->contains("stop_code") ){        s.stop_code = values.at( positions->indexOf("stop_code") ); }
                    if( positions->contains("stop_name") ){        s.stop_name = values.at( positions->indexOf("stop_name") ); }
                    if( positions->contains("stop_desc") ){        s.stop_desc = values.at( positions->indexOf("stop_desc") ); }
                    if( positions->contains("stop_lat") ){         s.stop_lat = values.at( positions->indexOf("stop_lat") ).toDouble(); }
                    if( positions->contains("stop_lon") ){         s.stop_lon = values.at( positions->indexOf("stop_lon") ).toDouble(); }
                    if( positions->contains("zone_id") ){          s.zone_id = values.at( positions->indexOf("zone_id") ).toInt(); }
                    if( positions->contains("stop_url") ){         s.stop_url = values.at( positions->indexOf("stop_url") ); }
                    if( positions->contains("location_type") ){    s.location_type = values.at( positions->indexOf("location_type") ).toInt(); }
                    if( positions->contains("parent_station") ){   s.parent_station = values.at( positions->indexOf("parent_station") ); }
                    if( positions->contains("stop_timezone") ){    s.stop_timezone = values.at( positions->indexOf("stop_timezone") ); }
                    if( positions->contains("wheelchair_boarding") ){s.wheelchair_boarding = values.at( positions->indexOf("wheelchair_boarding") ).toInt(); }
                    this->stops.insert(s.stop_id , s );
                }
            }
            delete positions;
        }
        file.close();
    }

    // Load Routes.txt
    {
        QStringList ss;
        ss << "route_id" << "agency_id" << "route_short_name" << "route_long_name" << "route_desc" << "route_type" << "route_url" << "route_color" << "route_text_color";
        QFile file( dir.absoluteFilePath("routes.txt") );
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QVector<QString>* positions = 0;
            QTextStream in(&file);
            while ( !in.atEnd() ) {
                QString line = in.readLine();
                QStringList values = line.split(',');
                if( !positions ){
                    positions = new QVector<QString>( values.size() );
                    foreach(QString s , ss){
                        if( values.contains(s) ){
                            positions->replace( values.indexOf(s) , s);
                        }
                    }
                } else {
                    Route r;
                    if( positions->contains("route_id") ){         r.route_id = values.at( positions->indexOf("route_id") ); }
                    if( positions->contains("agency_id") ){        r.agency_id = values.at( positions->indexOf("agency_id") ); }
                    if( positions->contains("route_short_name") ){ r.route_short_name = values.at( positions->indexOf("route_short_name") ); }
                    if( positions->contains("route_long_name") ){  r.route_long_name = values.at( positions->indexOf("route_long_name") ); }
                    if( positions->contains("route_desc") ){       r.route_desc = values.at( positions->indexOf("route_desc") ); }
                    if( positions->contains("route_type") ){       r.route_type = values.at( positions->indexOf("route_type") ).toInt(); }
                    if( positions->contains("route_url") ){        r.route_url = values.at( positions->indexOf("route_url") ); }
                    if( positions->contains("route_color") ){      r.route_color = values.at( positions->indexOf("route_color") ); }
                    if( positions->contains("route_text_color") ){ r.route_text_color = values.at( positions->indexOf("route_text_color") ); }
                    this->routes.insert(r.route_id , r );
                }
            }
            delete positions;
        }
        file.close();
    }

    // Load Trips.txt
    {
        QStringList ss;
        ss << "route_id" << "service_id" << "trip_id" << "trip_headsign" << "trip_short_name" << "direction_id" << "block_id" << "shape_id" << "wheelchair_accessible" << "bikes_allowed";
        QFile file( dir.absoluteFilePath("trips.txt") );
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QVector<QString>* positions = 0;
            QTextStream in(&file);
            while ( !in.atEnd() ) {
                QString line = in.readLine();
                QStringList values = line.split(',');
                if( !positions ){
                    positions = new QVector<QString>( values.size() );
                    foreach(QString s , ss){
                        if( values.contains(s) ){
                            positions->replace( values.indexOf(s) , s);
                        }
                    }
                } else {
                    Trip t;
                    if( positions->contains("route_id") ){         t.route_id = values.at( positions->indexOf("route_id") ); }
                    if( positions->contains("service_id") ){       t.service_id = values.at( positions->indexOf("service_id") ); }
                    if( positions->contains("trip_id") ){          t.trip_id = values.at( positions->indexOf("trip_id") ); }
                    if( positions->contains("trip_headsign") ){    t.trip_headsign = values.at( positions->indexOf("trip_headsign") ); }
                    if( positions->contains("trip_short_name") ){  t.trip_short_name = values.at( positions->indexOf("trip_short_name") ); }
                    if( positions->contains("direction_id") ){     t.direction_id = values.at( positions->indexOf("direction_id") ).toInt(); }
                    if( positions->contains("block_id") ){         t.block_id = values.at( positions->indexOf("block_id") ); }
                    if( positions->contains("shape_id") ){         t.shape_id = values.at( positions->indexOf("shape_id") ); }
                    if( positions->contains("wheelchair_accessible") ){ t.wheelchair_accessible = values.at( positions->indexOf("wheelchair_accessible") ).toInt(); }
                    if( positions->contains("bikes_allowed") ){    t.bikes_allowed = values.at( positions->indexOf("bikes_allowed") ).toInt(); }
                    this->trips.insert(t.trip_id , t );
                }
            }
            delete positions;
        }
        file.close();
    }

    // Load Stop_times.txt
    {
        QStringList ss;
        ss << "trip_id" << "arrival_time" << "departure_time" << "stop_id" << "stop_sequence" << "stop_headsign" << "pickup_type" << "drop_off_type" << "shape_dist_traveled" << "timepoint";
        QFile file( dir.absoluteFilePath("stop_times.txt") );
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QVector<QString>* positions = 0;
            QTextStream in(&file);
            while ( !in.atEnd() ) {
                QString line = in.readLine();
                QStringList values = line.split(',');
                if( !positions ){
                    positions = new QVector<QString>( values.size() );
                    foreach(QString s , ss){
                        if( values.contains(s) ){
                            positions->replace( values.indexOf(s) , s);
                        }
                    }
                } else {
                    StopTime s;
                    if( positions->contains("trip_id") ){          s.trip_id = values.at( positions->indexOf("trip_id") ); }
                    if( positions->contains("arrival_time") ){     s.arrival_time = QTime::fromString( values.at( positions->indexOf("arrival_time") ) , "hh:mm:ss"); }
                    if( positions->contains("departure_time") ){   s.departure_time = QTime::fromString( values.at( positions->indexOf("departure_time") ) , "hh:mm:ss"); }
                    if( positions->contains("stop_id") ){          s.stop_id = values.at( positions->indexOf("stop_id") ); }
                    if( positions->contains("stop_sequence") ){    s.stop_sequence = values.at( positions->indexOf("stop_sequence") ).toInt(); }
                    if( positions->contains("stop_headsign") ){    s.stop_headsign = values.at( positions->indexOf("stop_headsign") ); }
                    if( positions->contains("pickup_type") ){      s.pickup_type = values.at( positions->indexOf("pickup_type") ).toInt(); }
                    if( positions->contains("drop_off_type") ){    s.drop_off_type = values.at( positions->indexOf("drop_off_type") ).toInt(); }
                    if( positions->contains("shape_dist_traveled") ){ s.shape_dist_traveled = values.at( positions->indexOf("shape_dist_traveled") ).toDouble(); }
                    if( positions->contains("timepoint") ){        s.timepoint = values.at( positions->indexOf("timepoint") ).toInt(); }
                    QList<StopTime> ts = this->stoptimes.value(s.trip_id );
                    ts.append( s );
                    this->stoptimes.insert(s.trip_id , ts );
                }
            }
            delete positions;
        }
        file.close();
    }

    // Load calendar.txt
    {
        QStringList ss;
        ss << "service_id" << "monday" << "tuesday" << "wednesday" << "thursday" << "friday" << "saturday" << "sunday" << "start_date" << "end_date";
        QFile file( dir.absoluteFilePath("calendar.txt") );
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QVector<QString>* positions = 0;
            QTextStream in(&file);
            while ( !in.atEnd() ) {
                QString line = in.readLine();
                QStringList values = line.split(',');
                if( !positions ){
                    positions = new QVector<QString>( values.size() );
                    foreach(QString s , ss){
                        if( values.contains(s) ){
                            positions->replace( values.indexOf(s) , s);
                        }
                    }
                } else {
                    Calendar c;
                    if( positions->contains("service_id") ){       c.service_id = values.at( positions->indexOf("service_id") ); }
                    if( positions->contains("monday") ){           c.monday = values.at( positions->indexOf("monday") ).toInt() > 0 ? true : false ; }
                    if( positions->contains("tuesday") ){          c.tuesday = values.at( positions->indexOf("tuesday") ).toInt() > 0 ? true : false ; }
                    if( positions->contains("wednesday") ){        c.wednesday = values.at( positions->indexOf("wednesday") ).toInt() > 0 ? true : false ; }
                    if( positions->contains("thursday") ){         c.thursday = values.at( positions->indexOf("thursday") ).toInt() > 0 ? true : false ; }
                    if( positions->contains("friday") ){           c.friday = values.at( positions->indexOf("friday") ).toInt() > 0 ? true : false ; }
                    if( positions->contains("saturday") ){         c.saturday = values.at( positions->indexOf("saturday") ).toInt() > 0 ? true : false ; }
                    if( positions->contains("sunday") ){           c.sunday = values.at( positions->indexOf("sunday") ).toInt() > 0 ? true : false ; }
                    if( positions->contains("start_date") ){       c.start_date = QDate::fromString( values.at( positions->indexOf("start_date") ) , "yyyyMMdd" ); }
                    if( positions->contains("end_date") ){         c.end_date = QDate::fromString( values.at( positions->indexOf("end_date") ) , "yyyyMMdd" ); }
                    this->calendars.insert(c.service_id , c );
                }
            }
            delete positions;
        }
        file.close();
    }

    // Load calendar_dates.txt
    {
        QStringList ss;
        ss << "service_id" << "date" << "exception_type";
        QFile file( dir.absoluteFilePath("calendar_dates.txt") );
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QVector<QString>* positions = 0;
            QTextStream in(&file);
            while ( !in.atEnd() ) {
                QString line = in.readLine();
                QStringList values = line.split(',');
                if( !positions ){
                    positions = new QVector<QString>( values.size() );
                    foreach(QString s , ss){
                        if( values.contains(s) ){
                            positions->replace( values.indexOf(s) , s);
                        }
                    }
                } else {
                    CalendarDate c;
                    if( positions->contains("service_id") ){    c.service_id = values.at( positions->indexOf("service_id") ); }
                    if( positions->contains("date") ){          c.date = QDate::fromString( values.at( positions->indexOf("date") ) , "yyyyMMdd" ); }
                    if( positions->contains("exception_type") ){c.exception_type = values.at( positions->indexOf("exception_type") ).toInt(); }
                    QList<CalendarDate> ts = this->calendardates.value( c.service_id );
                    ts.append( c );
                    this->calendardates.insert( c.service_id , ts );
                }
            }
            delete positions;
        }
        file.close();
    }

    // Load shapes.txt
    {
        QStringList ss;
        ss << "shape_id" << "shape_pt_lat" << "shape_pt_lon" << "shape_pt_sequence" << "shape_dist_traveled";
        QFile file( dir.absoluteFilePath("shapes.txt") );
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QVector<QString>* positions = 0;
            QTextStream in(&file);
            while ( !in.atEnd() ) {
                QString line = in.readLine();
                QStringList values = line.split(',');
                if( !positions ){
                    positions = new QVector<QString>( values.size() );
                    foreach(QString s , ss){
                        if( values.contains(s) ){
                            positions->replace( values.indexOf(s) , s);
                        }
                    }
                } else {
                    Shape c;
                    if( positions->contains("shape_id") ){              c.shape_id = values.at( positions->indexOf("shape_id") ); }
                    if( positions->contains("shape_pt_lat") ){          c.shape_pt_lat = values.at( positions->indexOf("shape_pt_lat") ).toDouble(); }
                    if( positions->contains("shape_pt_lon") ){          c.shape_pt_lon = values.at( positions->indexOf("shape_pt_lon") ).toDouble(); }
                    if( positions->contains("shape_pt_sequence") ){     c.shape_pt_sequence = values.at( positions->indexOf("shape_pt_sequence") ).toDouble(); }
                    if( positions->contains("shape_dist_traveled") ){   c.shape_dist_traveled = values.at( positions->indexOf("shape_dist_traveled") ).toDouble(); }
                    QList<Shape> ts = this->shapes.value( c.shape_id );
                    ts.append( c );
                    this->shapes.insert( c.shape_id , ts );
                }
            }
            delete positions;
        }
        file.close();
    }

    qDebug() << "[GtfsImporter::GtfsImporter] Agencies:" << this->agencies.size()
             << ", Stops:" << this->stops.size()
             << ", Routes:" << this->routes.size()
             << ", Trips:" << this->trips.size()
             << ", StopTimes:" << this->stoptimes.size()
             << ", Calendars:" << this->calendars.size()
             << ", CalendarDates:" << this->calendardates.size()
             << ", Shapes:" << this->shapes.size();
}

GtfsImporter::~GtfsImporter(){

}

/**********************************************************************
 GETTERS
**********************************************************************/

QList<Agency> GtfsImporter::getAgencies(){
    return this->agencies.values();
}

Agency GtfsImporter::getAgencyById(QString id){
    return this->agencies.value(id);
}

QList<Stop> GtfsImporter::getStops(){
    return this->stops.values();
}

Stop GtfsImporter::getStopById(QString id){
    return this->stops.value(id);
}

QList<Route> GtfsImporter::getRoutes(){
    return this->routes.values();
}

Route GtfsImporter::getRouteById(QString id){
    return this->routes.value(id);
}

QList<Trip> GtfsImporter::getTrips(){
    return this->trips.values();
}

Trip GtfsImporter::getTripById(QString id){
    return this->trips.value(id);
}

QList<StopTime> GtfsImporter::getStopTimes(){
    QList<StopTime> st;
    foreach(QList<StopTime> s , this->stoptimes.values()){
        st.append( s );
    }
    return st;
}

QList<StopTime> GtfsImporter::getStopTimesByTripId(QString id){
    return this->stoptimes.value(id);
}

QList<Calendar> GtfsImporter::getCalendars(){
    return this->calendars.values();
}

Calendar GtfsImporter::getCalendarByServiceId(QString id){
    return this->calendars.value(id);
}

QList<CalendarDate> GtfsImporter::getCalendarDates(){
    QList<CalendarDate> st;
    foreach(QList<CalendarDate> s , this->calendardates.values()){
        st.append( s );
    }
    return st;
}

QList<CalendarDate> GtfsImporter::getCalendarDatesByServiceId(QString id){
    return this->calendardates.value(id);
}

QList<Shape> GtfsImporter::getShapes(){
    QList<Shape> st;
    foreach(QList<Shape> s , this->shapes.values()){
        st.append( s );
    }
    return st;
}

QList<Shape> GtfsImporter::getShapesByShapeId(QString id){
    return this->shapes.value(id);
}

/**********************************************************************
 METHODS
**********************************************************************/

QList<ItineraryVehicleDriver*> GtfsImporter::toDrivers(QList<PublicTransportStop*> snap_stops){

    QList<ItineraryVehicleDriver*> drivers;
    QMap<QString , PublicTransportStop*> snapped_stops; // STOP_ID , PublicTransportStop

    if( snap_stops.isEmpty() ){
        qWarning() << "[GtfsImporter::toDrivers] No stops where given, drivers will not be created";
        return drivers;
    }

    QList<Geometry*> points;
    foreach(Stop s , this->stops.values()){
        Point* p = Environment::getInstance()->getGeometryFactory()->createPoint( Coordinate( s.stop_lon , s.stop_lat ) );
        points.append(p);
    }

    QList<Agent*> as;
    foreach(PublicTransportStop* s , snap_stops){
        as.append( s );
    }

    as = Environment::getInstance()->getNearestAgents(points , as);
    for( int i = 0 ; i < this->stops.values().size() && i < as.size() ; i++){
        snapped_stops.insert( this->stops.values().at(i).stop_id , dynamic_cast<PublicTransportStop*>( as.at(i) ) );
        delete points.at(i);
    }

    foreach(Trip t , this->trips ){

        ItineraryVehicleDriver* driver = new ItineraryVehicleDriver();
        driver->setObjectName( t.trip_id );

        Route r = this->getRouteById( t.route_id );
        driver->setRouteName( r.route_long_name );
        Agency a = this->getAgencyById( r.agency_id );
        driver->setAgencyName( a.agency_name );

        QMap<int , StopTime> disordered_stop_times;

        // Get calendar
        Calendar c = this->getCalendarByServiceId( t.service_id );
        if( !c.service_id.isEmpty() ){
            QList<int> service_days;
            if( c.monday )     { service_days.append( 1 ); }
            if( c.tuesday )    { service_days.append( 2 ); }
            if( c.wednesday )  { service_days.append( 3 ); }
            if( c.thursday )   { service_days.append( 4 ); }
            if( c.friday )     { service_days.append( 5 ); }
            if( c.saturday )   { service_days.append( 6 ); }
            if( c.sunday )     { service_days.append( 7 ); }
            driver->setServiceDays( service_days );
            driver->setStartDate( c.start_date );
            driver->setEndDate( c.end_date );
        }

        // Get calendar dates
        foreach(CalendarDate c , this->getCalendarDatesByServiceId( t.service_id ) ){
            driver->setStartDate( c.date );
            driver->setEndDate( c.date );
        }

        // Order the stop times
        foreach(StopTime st , this->getStopTimesByTripId( t.trip_id )){
            disordered_stop_times.insert( st.stop_sequence , st );
        }

        // Get itinerary, arrivals and departures
        QList<ScheduledStop> itinerary;
        bool ended = false;
        int i = 0;

        while( !ended ){

            StopTime st;

            if( disordered_stop_times.keys().contains( ++i ) ){
                st = disordered_stop_times.value(i);
            } else if( disordered_stop_times.keys().contains( ++i ) ){ // SOME GTFS MIGHT SKIP ONE STOP
                st = disordered_stop_times.value(i);
            } else {
                ended = true;
            }

            if( snapped_stops.keys().contains( st.stop_id ) ){

                ScheduledStop scheduled_stop;
                scheduled_stop.stop = snapped_stops.value( st.stop_id );
                scheduled_stop.arrival = st.arrival_time;
                scheduled_stop.departure = st.departure_time;
                scheduled_stop.node = scheduled_stop.stop->getGraphNode();
                itinerary.append( scheduled_stop );

                if( !driver->getGeometry() ){
                    driver->setGeometry( snapped_stops.value( st.stop_id )->getGeometry()->clone() );
                }
            }
        }

        if( !itinerary.isEmpty() ){

            driver->setFromAndTo( itinerary );
            drivers.append( driver );

        } else {
            driver->deleteLater();
        }

    }
    return drivers;
}
