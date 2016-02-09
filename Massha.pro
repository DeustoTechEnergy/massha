#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT          += core
QT          += gui
QT          += network
QT          += websockets
QT          += sql
QT          += xml

# ACTIVATE QT-COMPILER OPTIMIZATION
QMAKE_CXXFLAGS += -march=native -O2 -pipe
QMAKE_CFLAGS += -march=native -O2 -pipe

# FORCE C++11
QMAKE_CXXFLAGS += -std=c++11

TARGET      = Massha
CONFIG      += console
CONFIG      -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
        # AGENTS
    agents/Agent.cpp \
            ## MOVING AGENT
    agents/moving/MovingAgent.cpp \
            ## PEOPLE
    agents/people/Person.cpp \
                ### ACTIVITY PEOPLE
    agents/people/activity_people/ActivityPerson.cpp \
            ## PLACES
    agents/places/Place.cpp \
            ## ROADS
    agents/roads/Road.cpp \
    agents/roads/RoadPoint.cpp \
            ## HOME APPLIANCES
    agents/objects/home_appliances/TimeCountdownAppliance.cpp \
    agents/objects/home_appliances/SwitchableAppliance.cpp \
        # UTILS
            ## PARALLELISM CONTROLLER
    utils/paralelism_controller/ParallelismController.cpp \
            ## HTTP SERVER
    utils/http_server/HttpServer.cpp \
    utils/http_server/RequestHandler.cpp \
    utils/http_server/RequestProcessor.cpp \
    utils/http_server/RequestProcessorCreator.cpp \
            ## WEBSOCKET SERVER
    utils/websocket_server/WebSocketServer.cpp \
            ## EXPORTERS
    #utils/exporters/agent_to_shop/AgentToShp.cpp \
    utils/exporters/geometry_to_geojson/GeometryToGeoJson.cpp \
    utils/exporters/logger/Logger.cpp \
    utils/exporters/agent_to_ui/AgentToUI.cpp \
            ## IMPORTERS
    #utils/importers/shp_importer/ShpImporter.cpp \
    utils/importers/json_importer/JsonImporter.cpp \
    utils/importers/geojson_to_geometry_importer/GeoJsonToGeometry.cpp \
    utils/importers/csv_importer/CsvImporter.cpp \
    utils/importers/gpx_importer/GpxImporter.cpp \
    utils/importers/ine_importer/IneMicroDataImporter.cpp \
    utils/importers/osm_importer/OsmImporter.cpp \
    utils/importers/tiff_importer/TiffImporter.cpp \
            ## UI STYLE
    utils/uistyle/UiStyle.cpp \
            ## GRAPHS
    utils/graph/Graph.cpp \
    utils/graph/GraphNode.cpp \
    utils/graph/GraphEdge.cpp \
        # ENVIRONMENT
    environment/Environment.cpp \
    environment/physical_environment/PhysicalEnvironment.cpp \
    environment/social_environment/SocialEnvironment.cpp \
    environment/social_environment/Message.cpp \
            ## TIME
    environment/time/TimeController.cpp \
        # SKILLS
    skills/Skill.cpp \
            ## COMMUNICATION
    skills/communication/CommunicateSkill.cpp \
            ## ALGORITHMS
    skills/linear_algebra/LeastSquaresSolvingSkill.cpp \
    skills/random_generators/NormalDistributionSkill.cpp \
    skills/random_generators/RouletteWheelSkill.cpp \
    skills/random_generators/UniformDistributionSkill.cpp \
            ## GRAPH
    skills/graph/GraphSkill.cpp \
    skills/graph/TspSkill.cpp \
    skills/graph/DijkstraSkill.cpp \
            ## CONTAIN
    skills/contain/AgentContainSkill.cpp \
        # ACTITIVIES
    activities/Activity.cpp \
    activities/SwitchableApplianceActivity.cpp \
    activities/TimeCountdownActivity.cpp

HEADERS += \
        # AGENTS
    agents/Agent.h \
            ## MOVING
    agents/moving/MovingAgent.h \
            ## PEOPLE
    agents/people/Person.h \
    agents/people/PersonConstants.h \
                ### ACTIVITY PEOPLE
    agents/people/activity_people/ActivityPerson.h \
            ## PLACES
    agents/places/Place.h \
            ## ROADS
    agents/roads/RoadPoint.h \
    agents/roads/Road.h \
    agents/roads/RoadConstants.h \
            ## HOME APPLIANCES
    agents/objects/home_appliances/TimeCountdownAppliance.h \
    agents/objects/home_appliances/SwitchableAppliance.h \
        # UTILS
            ## PARALLELISM CONTROLLER
    utils/paralelism_controller/ParallelismController.h \
            ## HTTP SERVER
    utils/http_server/HttpServer.h \
    utils/http_server/RequestHandler.h \
    utils/http_server/RequestProcessor.h \
    utils/http_server/RequestProcessorCreator.h \
            ## WEBSOCKET SERVER
    utils/websocket_server/WebSocketServer.h \
            ## EXPORTERS
    #utils/exporters/agent_to_shop/AgentToShp.h \
    utils/exporters/geometry_to_geojson/GeometryToGeoJson.h \
    utils/exporters/logger/Logger.h \
    utils/exporters/agent_to_ui/AgentToUI.h \
            ## IMPORTERS
    #utils/importers/shp_importer/ShpImporter.h \
    utils/importers/json_importer/JsonImporter.h \
    utils/importers/geojson_to_geometry_importer/GeoJsonToGeometry.h \
    utils/importers/csv_importer/CsvImporter.h \
    utils/importers/gpx_importer/GpxImporter.h \
    utils/importers/ine_importer/IneMicroDataImporter.h \
    utils/importers/osm_importer/OsmImporter.h \
    utils/importers/tiff_importer/TiffImporter.h \
            ## UI STYLE
    utils/uistyle/UiStyle.h \
            ## GRAPHS
    utils/graph/Graph.h \
    utils/graph/GraphNode.h \
    utils/graph/GraphEdge.h \
        # ENVIRONMENT
    environment/Environment.h \
    environment/physical_environment/PhysicalEnvironment.h \
    environment/social_environment/SocialEnvironment.h \
    environment/time/TimeController.h \
    environment/social_environment/Message.h \
        # SKILLS
    skills/SkillConstants.h \
    skills/Skill.h \
            ## COMMUNICATION
    skills/communication/CommunicateSkill.h \
            ## ALGORITHMS
    skills/linear_algebra/LeastSquaresSolvingSkill.h \
    skills/random_generators/NormalDistributionSkill.h \
    skills/random_generators/RouletteWheelSkill.h \
    skills/random_generators/UniformDistributionSkill.h \
            ## GRAPH
    skills/graph/GraphSkill.h \
    skills/graph/TspSkill.h \
    skills/graph/DijkstraSkill.h \
            ## CONTAIN
    skills/contain/AgentContainSkill.h \
        # ACTIVITIES
    activities/ActivityConstants.h \
    activities/Activity.h \
    activities/ActivityCreator.h \
    activities/SwitchableApplianceActivity.h \
    activities/TimeCountdownActivity.h


#INCLUDE QHTTPSERVER SYSTEM LIBRARY
unix|win32: LIBS += -lqhttpserver

#INCLUDE LIBGEOS SYSTEM LIBRARY
unix|win32: LIBS += -lgeos

#INCLUDE SHAPELIB SYSTEM LIBRARY
#unix|win32: LIBS += -lshp

#INCLUDE LEMON SYSTEM LIBRARY
unix|win32: LIBS += -lemon
