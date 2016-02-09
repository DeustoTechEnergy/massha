#include <QCoreApplication>
#include <QDebug>
#include <QSqlRecord>
#include <geos/io/WKTReader.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Point.h>
#include <geos/geom/CoordinateSequenceFactory.h>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDir>

#include "agents/Agent.h"
#include "environment/Environment.h"

#include "utils/exporters/logger/Logger.h"

#include "activities/ActivityCreator.h"

#include "agents/people/activity_people/ActivityPerson.h"
#include "agents/objects/home_appliances/SwitchableAppliance.h"
#include "agents/objects/home_appliances/TimeCountdownAppliance.h"
#include "utils/importers/csv_importer/CsvImporter.h"
#include "utils/importers/json_importer/JsonImporter.h"
#include "utils/importers/osm_importer/OsmImporter.h"

#include "agents/places/Place.h"
#include "utils/graph/GraphNode.h"
#include "utils/graph/GraphEdge.h"
#include "skills/random_generators/UniformDistributionSkill.h"
#include "skills/graph/DijkstraSkill.h"
#include "agents/roads/RoadPoint.h"

using namespace geos;

// INITIALIZE STATIC VARIABLES
QMap<ActivityConstants::activity_types, ActivityConstants::ActivityDescription > ActivityCreator::activity_descriptions;
QMap<ActivityConstants::activity_types, QList<ActivityConstants::ActivityDescription> > ActivityCreator::subactivity_descriptions;


int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    QCoreApplication app(argc, argv);

    // CREATE ENVIRONMENT
    Environment * environment = Environment::getInstance();
    environment->setGeometry(
                environment->getGeometryFromWKT(
                    QString("POLYGON((-2.948029339313507 43.27270499648687,-2.9476605355739594 43.27270499648687,-2.9476605355739594 43.27233003570001,-2.948029339313507 43.27233003570001,-2.948029339313507 43.27270499648687))\
                            ") ) ); // FICTICIAL
    environment->setEnvironmentTime( QDateTime(QDate::currentDate(), QTime(6, 20, 0, 0)) , 100 );

    /****************************************************************/
    // MASSHA : ACTIVITY SIMULATOR
    /****************************************************************/

    // CREATE THE HOUSE
    QMap<QString , RoadPoint*> path_points;
    QMap<QString , QString> sensors_icons;
    QMap<QString , Geometry*> places_geometries;
    QMap<QString , Geometry*> sensors_geometries;

    // Read twice, first for places and paths
    OsmImporter* osm = new OsmImporter("../doc/house.osm");
    foreach(OsmElement e , osm->getOsmElements()){

        // PLACE
        if( e.tags.keys().contains("place") ){
            places_geometries.insert( e.tags.value("place") , e.geometry->clone() );
        }

        // PATHS IN HOUSE
        if( e.tags.keys().contains("path") ){
            LineString* line = dynamic_cast<LineString*>( e.geometry );
            RoadPoint* last_roadpoint = 0;
            for(int i = 0; i < line->getNumPoints(); i++){
                Point* p = line->getPointN(i);
                QString hash = QString::fromStdString( p->toString() );
                RoadPoint* rp = path_points.value( hash, 0 );
                if( !rp ){
                    rp = new RoadPoint();
                    rp->setGeometry( p );
                    environment->addAgent( rp );
                    path_points.insert( hash , rp );
                }
                if( last_roadpoint ){
                    environment->physicalConnectNodes( last_roadpoint->getGraphNode() , rp->getGraphNode() , true );
                }
                last_roadpoint = rp;
            }
        }

        if( e.geometry ){ delete e.geometry; }
    }
    osm->deleteLater();

    // Second for sensors (they need the path to be constructed)
    osm = new OsmImporter("../doc/house.osm");
    foreach(OsmElement e , osm->getOsmElements()){

        // APPLIANCE
        if( e.tags.keys().contains("sensor") ){
            sensors_geometries.insert( e.tags.value("sensor") , e.geometry->clone() );
        }

        if( e.geometry ){ delete e.geometry; }
    }
    osm->deleteLater();

    // KITCHEN
    Place* kitchen = 0;
    if( places_geometries.keys().contains("KITCHEN") ){
        kitchen = new Place();
        kitchen->setObjectName("KITCHEN");
        kitchen->setGeometry( places_geometries.value("KITCHEN") );
        kitchen->getStyle()->color = QColor("White");
        environment->addAgent( kitchen );

        sensors_icons.clear();
        sensors_icons.insert("Microwave_115" , "http://cdn.flaticon.com/png/128/93946.png");
        sensors_icons.insert("Microwave_143" , "http://cdn.flaticon.com/png/128/93946.png");
        sensors_icons.insert("Oven_129" , "http://cdn.flaticon.com/png/128/93946.png");
        sensors_icons.insert("Dishwasher_70" , "http://cdn.flaticon.com/png/128/15138.png");
        sensors_icons.insert("Toaster_108" , "http://cdn.flaticon.com/png/128/1/1089.png");
        sensors_icons.insert("Toaster_131" , "http://cdn.flaticon.com/png/128/1/1089.png");
        sensors_icons.insert("WashingMachine_142" , "http://cdn.flaticon.com/png/128/79708.png");
        sensors_icons.insert("LaundryDryer_90" , "http://cdn.flaticon.com/png/128/14116.png");
        foreach(QString key , sensors_icons.keys()){
            if( sensors_geometries.keys().contains(key) ){
                TimeCountdownAppliance* t = new TimeCountdownAppliance();
                t->setObjectName(key);
                t->setGeometry( sensors_geometries.value(key , 0) );
                t->getStyle()->icon_url = sensors_icons.value(key);
                environment->addAgent(t);
                // Find its nearest road point and connect it to it
                Agent* nearest = environment->getNearestAgent( t->getGeometry() , "RoadPoint" );
                environment->physicalConnectNodes( nearest->getGraphNode() , t->getGraphNode() , true );
            }
        }
        sensors_icons.clear();
        sensors_icons.insert("Burner_94" , "http://cdn.flaticon.com/png/128/94004.png");
        sensors_icons.insert("Burner_99" , "http://cdn.flaticon.com/png/128/94004.png");
        sensors_icons.insert("Burner_106" , "http://cdn.flaticon.com/png/128/94004.png");
        sensors_icons.insert("Burner_117" , "http://cdn.flaticon.com/png/128/94004.png");
        sensors_icons.insert("Burner_118" , "http://cdn.flaticon.com/png/128/94004.png");
        sensors_icons.insert("Cabinet_51" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_54" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_64" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_104" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_111" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_116" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_53" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_55" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_59" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_61" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_63" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_66" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_72" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_73" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_80" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_83" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_89" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_132" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_133" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_138" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cereal_145" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Coffeemachine_119" , "http://cdn.flaticon.com/png/128/81/81305.png");
        sensors_icons.insert("Containers_60" , "http://cdn.flaticon.com/png/128/12960.png");
        sensors_icons.insert("Containers_124" , "http://cdn.flaticon.com/png/128/12960.png");
        sensors_icons.insert("Door_51" , "http://cdn.flaticon.com/png/128/32567.png");
        sensors_icons.insert("Door_54" , "http://cdn.flaticon.com/png/128/32567.png");
        sensors_icons.insert("Door_141" , "http://cdn.flaticon.com/png/128/32567.png");
        sensors_icons.insert("Drawer_52" , "http://cdn.flaticon.com/png/128/47/47510.png");
        sensors_icons.insert("Drawer_69" , "http://cdn.flaticon.com/png/128/47/47510.png");
        sensors_icons.insert("Drawer_78" , "http://cdn.flaticon.com/png/128/47/47510.png");
        sensors_icons.insert("Drawer_84" , "http://cdn.flaticon.com/png/128/47/47510.png");
        sensors_icons.insert("Drawer_87" , "http://cdn.flaticon.com/png/128/47/47510.png");
        sensors_icons.insert("Drawer_114" , "http://cdn.flaticon.com/png/128/47/47510.png");
        sensors_icons.insert("Drawer_125" , "http://cdn.flaticon.com/png/128/47/47510.png");
        sensors_icons.insert("Drawer_135" , "http://cdn.flaticon.com/png/128/47/47510.png");
        sensors_icons.insert("Freezer_70" , "http://cdn.flaticon.com/png/128/14511.png");
        sensors_icons.insert("Freezer_137" , "http://cdn.flaticon.com/png/128/14511.png");
        sensors_icons.insert("Garbagedisposal_84" , "http://cdn.flaticon.com/png/128/12960.png");
        sensors_icons.insert("Garbagedisposal_98" , "http://cdn.flaticon.com/png/128/12960.png");
        sensors_icons.insert("Lightswitch_75" , "http://cdn.flaticon.com/png/128/46754.png");
        sensors_icons.insert("Lightswitch_119" , "http://cdn.flaticon.com/png/128/46754.png");
        sensors_icons.insert("Lightswitch_95" , "http://cdn.flaticon.com/png/128/46754.png");
        sensors_icons.insert("Lightswitch_105" , "http://cdn.flaticon.com/png/128/46754.png");
        sensors_icons.insert("Lightswitch_120" , "http://cdn.flaticon.com/png/128/46754.png");
        sensors_icons.insert("Refrigerator_91" , "http://cdn.flaticon.com/png/128/14511.png");
        sensors_icons.insert("Refrigerator_126" , "http://cdn.flaticon.com/png/128/14511.png");
        sensors_icons.insert("Refrigerator_144" , "http://cdn.flaticon.com/png/128/14511.png");
        sensors_icons.insert("Refrigerator_66" , "http://cdn.flaticon.com/png/128/14511.png");
        sensors_icons.insert("Refrigerator_74" , "http://cdn.flaticon.com/png/128/14511.png");
        sensors_icons.insert("Refrigerator_87" , "http://cdn.flaticon.com/png/128/14511.png");
        sensors_icons.insert("Refrigerator_97" , "http://cdn.flaticon.com/png/128/14511.png");
        sensors_icons.insert("Sinkfaucet-cold_91" , "http://cdn.flaticon.com/png/128/27591.png");
        sensors_icons.insert("Sinkfaucet-cold_96" , "http://cdn.flaticon.com/png/128/27591.png");
        sensors_icons.insert("Sinkfaucet-cold_118" , "http://cdn.flaticon.com/png/128/27591.png");
        foreach(QString key , sensors_icons.keys()){
            if( sensors_geometries.keys().contains(key) ){
                SwitchableAppliance* t = new SwitchableAppliance();
                t->setObjectName(key);
                t->setGeometry( sensors_geometries.value(key , 0) );
                t->getStyle()->icon_url = sensors_icons.value(key);
                environment->addAgent(t);
                // Find its nearest road point and connect it to it
                Agent* nearest = environment->getNearestAgent( t->getGeometry() , "RoadPoint" );
                environment->physicalConnectNodes( nearest->getGraphNode() , t->getGraphNode() , true );
            }
        }
    }

    // BATHROOM
    Place* bathroom = 0;
    if( places_geometries.keys().contains("BATHROOM") ){
        bathroom = new Place();
        bathroom->setObjectName("BATHROOM");
        bathroom->setGeometry( places_geometries.value("BATHROOM") );
        bathroom->getStyle()->color = QColor("Lightblue");
        environment->addAgent( bathroom );

        sensors_icons.clear();
        sensors_icons.insert("ToiletFlush_100" , "http://cdn.flaticon.com/png/128/13316.png");
        sensors_icons.insert("ToiletFlush_112" , "http://cdn.flaticon.com/png/128/13316.png");
        foreach(QString key , sensors_icons.keys()){
            if( sensors_geometries.keys().contains(key) ){
                TimeCountdownAppliance* t = new TimeCountdownAppliance();
                t->setObjectName(key);
                t->setGeometry( sensors_geometries.value(key , 0) );
                t->getStyle()->icon_url = sensors_icons.value(key);
                environment->addAgent(t);
                // Find its nearest road point and connect it to it
                Agent* nearest = environment->getNearestAgent( t->getGeometry() , "RoadPoint" );
                environment->physicalConnectNodes( nearest->getGraphNode() , t->getGraphNode() , true );
            }
        }
        sensors_icons.clear();
        sensors_icons.insert("Cabinet_67" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_79" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Door_130" , "http://cdn.flaticon.com/png/128/32567.png");
        sensors_icons.insert("Door_137" , "http://cdn.flaticon.com/png/128/32567.png");
        sensors_icons.insert("Hamper_78" , "http://cdn.flaticon.com/png/128/58271.png");
        sensors_icons.insert("ExhaustFan_96" , "http://cdn.flaticon.com/png/128/1281.png");
        sensors_icons.insert("Lightswitch_101" , "http://cdn.flaticon.com/png/128/46754.png");
        sensors_icons.insert("Lightswitch_109" , "http://cdn.flaticon.com/png/128/46754.png");
        sensors_icons.insert("Medicinecabinet_57" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Medicinecabinet_58" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Medicinecabinet_127" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Medicinecabinet_139" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Showerfaucet_93" , "http://cdn.flaticon.com/png/128/70/70036.png");
        sensors_icons.insert("Showerfaucet_60" , "http://cdn.flaticon.com/png/128/70/70036.png");
        sensors_icons.insert("Showerfaucet_130" , "http://cdn.flaticon.com/png/128/70/70036.png");
        sensors_icons.insert("Sinkfaucet-hot_68" , "http://cdn.flaticon.com/png/128/27591.png");
        sensors_icons.insert("Sinkfaucet-hot_100" , "http://cdn.flaticon.com/png/128/27591.png");
        sensors_icons.insert("Sinkfaucet-cold_88" , "http://cdn.flaticon.com/png/128/27591.png");
        sensors_icons.insert("Sinkfaucet-cold_94" , "http://cdn.flaticon.com/png/128/27591.png");
        foreach(QString key , sensors_icons.keys()){
            if( sensors_geometries.keys().contains(key) ){
                SwitchableAppliance* t = new SwitchableAppliance();
                t->setObjectName(key);
                t->setGeometry( sensors_geometries.value(key , 0) );
                t->getStyle()->icon_url = sensors_icons.value(key);
                environment->addAgent(t);
                // Find its nearest road point and connect it to it
                Agent* nearest = environment->getNearestAgent( t->getGeometry() , "RoadPoint" );
                environment->physicalConnectNodes( nearest->getGraphNode() , t->getGraphNode() , true );
            }
        }
    }

    // FOYER
    Place* foyer = 0;
    if( places_geometries.keys().contains("FOYER") ){
        foyer = new Place();
        foyer->setObjectName("FOYER");
        foyer->setGeometry( places_geometries.value("FOYER") );
        foyer->getStyle()->color = QColor("Peru");
        environment->addAgent( foyer );

        sensors_icons.clear();
        sensors_icons.insert("Door_140" , "http://cdn.flaticon.com/png/128/32567.png");
        sensors_icons.insert("Closet_81" , "http://cdn.flaticon.com/png/128/47428.png");
        sensors_icons.insert("Lightswitch_104" , "http://cdn.flaticon.com/png/128/46754.png");
        foreach(QString key , sensors_icons.keys()){
            if( sensors_geometries.keys().contains(key) ){
                SwitchableAppliance* t = new SwitchableAppliance();
                t->setObjectName(key);
                t->setGeometry( sensors_geometries.value(key , 0) );
                t->getStyle()->icon_url = sensors_icons.value(key);
                environment->addAgent(t);
                // Find its nearest road point and connect it to it
                Agent* nearest = environment->getNearestAgent( t->getGeometry() , "RoadPoint" );
                environment->physicalConnectNodes( nearest->getGraphNode() , t->getGraphNode() , true );
            }
        }
    }

    // BEDROOM
    Place* bedroom = 0;
    if( places_geometries.keys().contains("BEDROOM") ){
        bedroom = new Place();
        bedroom->setObjectName("BEDROOM");
        bedroom->setGeometry( places_geometries.value("BEDROOM") );
        bedroom->getStyle()->color = QColor("MediumTurquoise");
        environment->addAgent( bedroom );

        sensors_icons.clear();
        sensors_icons.insert("Drawer_62" , "http://cdn.flaticon.com/png/128/47/47510.png");
        sensors_icons.insert("Drawer_71" , "http://cdn.flaticon.com/png/128/47/47510.png");
        sensors_icons.insert("Drawer_75" , "http://cdn.flaticon.com/png/128/47/47510.png");
        sensors_icons.insert("Drawer_86" , "http://cdn.flaticon.com/png/128/47/47510.png");
        sensors_icons.insert("Drawer_146" , "http://cdn.flaticon.com/png/128/47/47510.png");
        sensors_icons.insert("Jewelrybox_139" , "http://cdn.flaticon.com/png/128/1695.png");
        sensors_icons.insert("Lamp_64" , "http://cdn.flaticon.com/png/128/9123.png");
        sensors_icons.insert("Lightswitch_106" , "http://cdn.flaticon.com/png/128/46754.png");
        sensors_icons.insert("Lightswitch_108" , "http://cdn.flaticon.com/png/128/46754.png");
        sensors_icons.insert("TV_101" , "http://cdn.flaticon.com/png/128/21392.png");
        foreach(QString key , sensors_icons.keys()){
            if( sensors_geometries.keys().contains(key) ){
                SwitchableAppliance* t = new SwitchableAppliance();
                t->setObjectName(key);
                t->setGeometry( sensors_geometries.value(key , 0) );
                t->getStyle()->icon_url = sensors_icons.value(key);
                environment->addAgent(t);
                // Find its nearest road point and connect it to it
                Agent* nearest = environment->getNearestAgent( t->getGeometry() , "RoadPoint" );
                environment->physicalConnectNodes( nearest->getGraphNode() , t->getGraphNode() , true );
            }
        }
    }

    // LIVING ROOM
    Place* livingroom = 0;
    if( places_geometries.keys().contains("LIVING_ROOM") ){
        livingroom = new Place();
        livingroom->setObjectName("LIVING_ROOM");
        livingroom->setGeometry( places_geometries.value("LIVING_ROOM") );
        livingroom->getStyle()->color = QColor("LightSalmon");
        environment->addAgent( livingroom );

        sensors_icons.clear();
        sensors_icons.insert("DVD_56" , "http://cdn.flaticon.com/png/128/47738.png");
        sensors_icons.insert("Lamp_76" , "http://cdn.flaticon.com/png/128/9123.png");
        sensors_icons.insert("Lightswitch_97" , "http://cdn.flaticon.com/png/128/46754.png");
        sensors_icons.insert("Lightswitch_107" , "http://cdn.flaticon.com/png/128/46754.png");
        sensors_icons.insert("Lightswitch_102" , "http://cdn.flaticon.com/png/128/46754.png");
        sensors_icons.insert("Lightswitch_110" , "http://cdn.flaticon.com/png/128/46754.png");
        sensors_icons.insert("TV_141" , "http://cdn.flaticon.com/png/128/21392.png");
        foreach(QString key , sensors_icons.keys()){
            if( sensors_geometries.keys().contains(key) ){
                SwitchableAppliance* t = new SwitchableAppliance();
                t->setObjectName(key);
                t->setGeometry( sensors_geometries.value(key , 0) );
                t->getStyle()->icon_url = sensors_icons.value(key);
                environment->addAgent(t);
                // Find its nearest road point and connect it to it
                Agent* nearest = environment->getNearestAgent( t->getGeometry() , "RoadPoint" );
                environment->physicalConnectNodes( nearest->getGraphNode() , t->getGraphNode() , true );
            }
        }
    }

    // OFFICE
    Place* office = 0;
    if( places_geometries.keys().contains("OFFICE") ){
        office = new Place();
        office->setObjectName("OFFICE");
        office->setGeometry( places_geometries.value("OFFICE") );
        office->getStyle()->color = QColor("LightGray");
        environment->addAgent( office );

        sensors_icons.clear();
        sensors_icons.insert("Door_133" , "http://cdn.flaticon.com/png/128/32567.png");
        sensors_icons.insert("Drawer_68" , "http://cdn.flaticon.com/png/128/47/47510.png");
        //sensors_icons.insert("Drawer_71" , "http://cdn.flaticon.com/png/128/47/47510.png");
        sensors_icons.insert("Drawer_82" , "http://cdn.flaticon.com/png/128/47/47510.png"); // Repeated in both datasets
        //sensors_icons.insert("Drawer_135" , "http://cdn.flaticon.com/png/128/47/47510.png"); // Repeated in both datasets
        sensors_icons.insert("Lightswitch_92" , "http://cdn.flaticon.com/png/128/46754.png");
        //sensors_icons.insert("Lightswitch_107" , "http://cdn.flaticon.com/png/128/46754.png");
        foreach(QString key , sensors_icons.keys()){
            if( sensors_geometries.keys().contains(key) ){
                SwitchableAppliance* t = new SwitchableAppliance();
                t->setObjectName(key);
                t->setGeometry( sensors_geometries.value(key , 0) );
                t->getStyle()->icon_url = sensors_icons.value(key);
                environment->addAgent(t);
                // Find its nearest road point and connect it to it
                Agent* nearest = environment->getNearestAgent( t->getGeometry() , "RoadPoint" );
                environment->physicalConnectNodes( nearest->getGraphNode() , t->getGraphNode() , true );
            }
        }
    }

    // BUTLERS PANTRY
    Place* pantry = 0;
    if( places_geometries.keys().contains("BUTLERS_PANTRY") ){
        pantry = new Place();
        pantry->setObjectName("BUTLERS_PANTRY");
        pantry->setGeometry( places_geometries.value("BUTLERS_PANTRY") );
        pantry->getStyle()->color = UiStyle::randomColor();
        environment->addAgent( pantry );

        sensors_icons.clear();
        sensors_icons.insert("Cabinet_57" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_67" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_73" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_76" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_79" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_138" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_142" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Drawer_63" , "http://cdn.flaticon.com/png/128/47/47510.png");
        sensors_icons.insert("Drawer_77" , "http://cdn.flaticon.com/png/128/47/47510.png");
        sensors_icons.insert("Drawer_89" , "http://cdn.flaticon.com/png/128/47/47510.png");
        sensors_icons.insert("Lightswitch_53" , "http://cdn.flaticon.com/png/128/46754.png");
        foreach(QString key , sensors_icons.keys()){
            if( sensors_geometries.keys().contains(key) ){
                SwitchableAppliance* t = new SwitchableAppliance();
                t->setObjectName(key);
                t->setGeometry( environment->getRandomPoint( pantry->getGeometry() ) );
                t->getStyle()->icon_url = sensors_icons.value(key);
                environment->addAgent(t);
                // Find its nearest road point and connect it to it
                Agent* nearest = environment->getNearestAgent( t->getGeometry() , "RoadPoint" );
                environment->physicalConnectNodes( nearest->getGraphNode() , t->getGraphNode() , true );
            }
        }
    }

    // DEN
    Place* den = 0;
    if( places_geometries.keys().contains("DEN") ){
        den = new Place();
        den->setObjectName("DEN");
        den->setGeometry( places_geometries.value("DEN") );
        den->getStyle()->color = UiStyle::randomColor();
        environment->addAgent( den );

        sensors_icons.clear();
        sensors_icons.insert("Door_134" , "http://cdn.flaticon.com/png/128/32567.png");
        sensors_icons.insert("Drawer_126" , "http://cdn.flaticon.com/png/128/47/47510.png");
        sensors_icons.insert("Drawer_128" , "http://cdn.flaticon.com/png/128/47/47510.png");
        sensors_icons.insert("Lightswitch_98" , "http://cdn.flaticon.com/png/128/46754.png");
        sensors_icons.insert("Lightswitch_120" , "http://cdn.flaticon.com/png/128/46754.png");
        sensors_icons.insert("Stereo_122" , "http://cdn.flaticon.com/png/128/79/79419.png");
        sensors_icons.insert("Telephone_69" , "http://cdn.flaticon.com/png/128/15866.png");
        foreach(QString key , sensors_icons.keys()){
            if( sensors_geometries.keys().contains(key) ){
                SwitchableAppliance* t = new SwitchableAppliance();
                t->setObjectName(key);
                t->setGeometry( environment->getRandomPoint( den->getGeometry() ) );
                t->getStyle()->icon_url = sensors_icons.value(key);
                environment->addAgent(t);
                // Find its nearest road point and connect it to it
                Agent* nearest = environment->getNearestAgent( t->getGeometry() , "RoadPoint" );
                environment->physicalConnectNodes( nearest->getGraphNode() , t->getGraphNode() , true );
            }
        }
    }

    // DINNING ROOM
    Place* dinning = 0;
    if( places_geometries.keys().contains("DINNING_ROOM") ){
        dinning = new Place();
        dinning->setObjectName("DINNING_ROOM");
        dinning->setGeometry( places_geometries.value("DINNING_ROOM") );
        dinning->getStyle()->color = UiStyle::randomColor();
        environment->addAgent( dinning );

        sensors_icons.clear();
        sensors_icons.insert("Cabinet_61" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Cabinet_140" , "http://cdn.flaticon.com/png/128/15408.png");
        sensors_icons.insert("Drawer_72" , "http://cdn.flaticon.com/png/128/47/47510.png");
        sensors_icons.insert("Drawer_132" , "http://cdn.flaticon.com/png/128/47/47510.png");
        sensors_icons.insert("Lightswitch_145" , "http://cdn.flaticon.com/png/128/46754.png");
        foreach(QString key , sensors_icons.keys()){
            if( sensors_geometries.keys().contains(key) ){
                SwitchableAppliance* t = new SwitchableAppliance();
                t->setObjectName(key);
                t->setGeometry( environment->getRandomPoint( dinning->getGeometry() ) );
                t->getStyle()->icon_url = sensors_icons.value(key);
                environment->addAgent(t);
                // Find its nearest road point and connect it to it
                Agent* nearest = environment->getNearestAgent( t->getGeometry() , "RoadPoint" );
                environment->physicalConnectNodes( nearest->getGraphNode() , t->getGraphNode() , true );
            }
        }
    }

    // HALLWAY
    Place* hallway = 0;
    if( places_geometries.keys().contains("HALLWAY") ){
        hallway = new Place();
        hallway->setObjectName("HALLWAY");
        hallway->setGeometry( places_geometries.value("HALLWAY") );
        hallway->getStyle()->color = UiStyle::randomColor();
        environment->addAgent( hallway );

        sensors_icons.clear();
        sensors_icons.insert("Door_85" , "http://cdn.flaticon.com/png/128/32567.png");
        sensors_icons.insert("Lightswitch_95" , "http://cdn.flaticon.com/png/128/46754.png");
        sensors_icons.insert("Lightswitch_99" , "http://cdn.flaticon.com/png/128/46754.png");
        sensors_icons.insert("Lightswitch_103" , "http://cdn.flaticon.com/png/128/46754.png");
        foreach(QString key , sensors_icons.keys()){
            if( sensors_geometries.keys().contains(key) ){
                SwitchableAppliance* t = new SwitchableAppliance();
                t->setObjectName(key);
                t->setGeometry( environment->getRandomPoint( hallway->getGeometry() ) );
                t->getStyle()->icon_url = sensors_icons.value(key);
                environment->addAgent(t);
                // Find its nearest road point and connect it to it
                Agent* nearest = environment->getNearestAgent( t->getGeometry() , "RoadPoint" );
                environment->physicalConnectNodes( nearest->getGraphNode() , t->getGraphNode() , true );
            }
        }
    }

    // ACTIVITIES FILE

    QString input_file = QString("demo_video.json").arg( argv[1] ).arg( argv[2] );
    qDebug() << "[main] Source configuration" << input_file;

    QString log_file = QString("logsim-demo.csv").arg( argv[1] ).arg( argv[2] );
    qDebug() << "[main] Log file" << log_file;


    // LOAD CONFIGURATION FILE
    JsonImporter* json_importer = new JsonImporter("../doc/" + input_file );
    QJsonObject json = json_importer->getJson();


    // PARSE ACTIVITY CONFIGURATION
    QJsonArray activity_configuration = json["activity_configuration"].toArray();

    for( int i = 0; i < activity_configuration.size(); i++ ){
        QJsonObject json_activity = activity_configuration.at(i).toObject();
        QJsonArray subactivities = json_activity["subactivities"].toArray();

        // CREATE ACTIVITY
        ActivityConstants::ActivityDescription activity = ActivityCreator::createActivityDescription( json_activity );
        if ( activity.activity_type != -1 ) {
            ActivityCreator::addActivityDescription( activity );

            qDebug() << "[main] Creating Activity:" << ActivityConstants::activityTypeToString( activity.activity_type );

            // CREATE SUBACTIVITIES
            for( int j = 0; j < subactivities.size(); j++ ){
                QJsonObject json_subactivity = subactivities.at(j).toObject();

                ActivityConstants::ActivityDescription subactivity = ActivityCreator::createActivityDescription( json_subactivity );
                if ( subactivity.activity_type != -1 ) {
                    ActivityCreator::addSubactivityDescription( activity.activity_type, subactivity );
                    qDebug() << "[main] - Adding subactivity" << ActivityConstants::activityTypeToString( subactivity.activity_type );
                }
            }
        }
    }
    qDebug() << endl;

    Logger::log("id,timestamp,sensor_id,sensor_name,sens_event,activity,act_event,duration", log_file);

    // PARSE TODO_LIST
    QJsonArray todo_list = json["todo_list"].toArray();

    for( int j = 0 ; j < 1 ; j++ ){

        // CREATE THE PERSON
        ActivityPerson* person = new ActivityPerson();
        person->setObjectName( log_file );
        // Get a random ROadPOint
        person->setGeometry( environment->getAgents("RoadPoint").at( qrand() % environment->getAgents("RoadPoint").size() )->getGeometry()->clone() );
        environment->addAgent( person );

        for( int i = 0; i < todo_list.size() ; i++){
            QJsonObject json_todo = todo_list.at(i).toObject();
            Activity* a = ActivityCreator::createActivity( ActivityConstants::stringToActivityType( json_todo["activity_type"].toString() ), person );

            if ( !a ){
                continue;
            }

            // OVERWRITE the default activity attributes if new ones are specified
            a->setFrequency( json_todo["frequency"].toDouble( a->getFrequency() ) );
            a->setFinishObjective( json_todo["finish_objective"].toDouble( a->getFinishObjective() ) );
            a->setFinishIncrement( json_todo["finish_increment"].toDouble( a->getFinishIncrement() ) );
            a->setFrequency( json_todo["frequency"].toDouble( a->getFrequency() ) );
            a->setPriorityBase( json_todo["priority_base"].toDouble( a->getPriorityBase() ) );
            a->setPriorityIncrement( json_todo["priority_increment"].toDouble( a->getPriorityIncrement() ) );
            a->setDesiredStartTime( QTime::fromString( json_todo["desired_start_time"].toString( a->getDesiredStartTime().toString("hh:mm") ) , "hh:mm" ) );
            a->setDesiredEndTime( QTime::fromString( json_todo["desired_end_time"].toString( a->getDesiredEndTime().toString( "hh:mm" ) ) , "hh:mm" ) );
            a->setMandatoryStartTime( QTime::fromString( json_todo["mandatory_start_time"].toString( a->getMandatoryStartTime().toString( "hh:mm" ) ) , "hh:mm" ) );
            a->setMandatoryEndTime( QTime::fromString( json_todo["mandatory_end_time"].toString( a->getMandatoryEndTime().toString( "hh:mm" ) ) , "hh:mm" ) );
            a->setSensorName( json_todo["sensor_name"].toString( a->getSensorName() ) );

            if ( a->hasMandatoryTime() ){
                a->setEstimatedDuration( a->getMandatoryStartTime().secsTo( a->getMandatoryEndTime() ) );
            }

            if ( json_todo["preconditions"].isArray() ){
                foreach ( QJsonValue p, json_todo["preconditions"].toArray() ){
                    a->addPrecondition( ActivityConstants::stringToActivityType( p.toString() ));
                }
            }
            if ( json_todo["outcomes"].isArray() ){
                foreach ( QJsonValue p, json_todo["outcomes"].toArray() ){
                    a->addOutcome( ActivityConstants::stringToActivityType( p.toString() ));
                }
            }
            if ( json_todo["consumes"].isArray() ){
                foreach ( QJsonValue p, json_todo["consumes"].toArray() ){
                    a->addConsume( ActivityConstants::stringToActivityType( p.toString() ));
                }
            }

            qDebug() << "[main] Adding TODO activity" << ActivityConstants::activityTypeToString( a->getType() );
            person->addToDoActivity( a );
        }

        //QTimer::singleShot(0, person, SLOT(run()));
    }
    qDebug() << endl << "MASSHA ready, access from http://localhost:3000" << endl;


    // EXECUTE APP
    app.exec();
}
