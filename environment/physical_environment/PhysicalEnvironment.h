#ifndef PHYSICALENVIRONMENT_H
#define PHYSICALENVIRONMENT_H

#include <QMutex>

#include <geos/geom/GeometryFactory.h>

#include <geos/geom/Geometry.h>
#include <geos/geom/Polygon.h>

#include <geos/index/quadtree/Quadtree.h>
#include <geos/index/strtree/STRtree.h>

#include "agents/Agent.h"
#include "utils/graph/Graph.h"

using namespace geos::geom;
using namespace geos::index::quadtree;
using namespace geos::index::strtree;
using namespace geos::planargraph;

class PhysicalEnvironment
{

public:

    QJsonObject physicalToJSON();

    // GETTERS
    Point* getRandomPoint( geos::geom::Geometry* bounds = 0, int seed = qrand() );
    geos::geom::Geometry* getGeometryFromWKT( QString wkt );
    QString getWKTFromGeometry( geos::geom::Geometry* geometry );
    geos::geom::GeometryFactory* getGeometryFactory();
    QList<Agent*> orderByDistance( Agent* source , QList<Agent*> agents );
    QList<Agent*> getAgentsByGeometry( geos::geom::Geometry* geometry, QString class_name );
    Agent* getNearestAgent( geos::geom::Geometry* geometry, QString class_name );
    Agent* getNearestAgent( geos::geom::Geometry* geometry, QList<Agent*> agents );
    QList<Agent*> getNearestAgents( QList<geos::geom::Geometry*> geometries, QString class_name );
    QList<Agent*> getNearestAgents( QList<geos::geom::Geometry*> geometries, QList<Agent*> agents );
    Agent* getAgentByGeometry( geos::geom::Geometry* geometry, QString class_name );

    // PHYSICAL GRAPH METHODS
    bool arePhysicallyConnected( GraphNode* from , GraphNode* to );
    GraphEdge* physicalConnectNodes( GraphEdge* edge );
    GraphEdge* physicalConnectNodes( GraphNode* from , GraphNode* to , bool bidirection = false );
    void physycalDisconnectNodes( GraphNode* from , GraphNode* to);
    QMap<GraphEdge* , double> getPhysicalGraph();

protected:
    explicit PhysicalEnvironment();
    ~PhysicalEnvironment();

    // AGENT METHODS
    bool addAgent( Agent* agent );
    geos::geom::Geometry* updateAgentGeometry( Agent* agent, geos::geom::Geometry* old_geom, geos::geom::Geometry* new_geom ); // Update agent geometry
    bool removeAgent( Agent* agent );

private:
    geos::geom::GeometryFactory* geometry_factory; // Geometry factory
    QHash<QString, Quadtree*> spatial_index; // Spatial indexes
    Graph* spatial_graph; // Spatial graph

    // Mutex, for avoiding concurrency
    QMutex mutex;
};

#endif // PHYSICALENVIRONMENT_H
