#ifndef GRAPH_H
#define GRAPH_H

#include "geos/planargraph/PlanarGraph.h"
#include "geos/geom/MultiLineString.h"
#include "utils/graph/GraphNode.h"
#include "utils/graph/GraphEdge.h"

using namespace geos::geom;
using namespace geos::planargraph;

class Graph : public PlanarGraph
{

public:
    explicit Graph();

    // GETTERS
    GraphNode* findNode( Coordinate coor );
    GraphEdge* findEdge( GraphNode* from , GraphNode* to );
    QList<GraphNode*> getNodes();
    QList<GraphEdge*> getEdges();
    int countNodes();
    int countEdges();
    MultiLineString* toMultiLineString();

    // SETTERS
    void addNode( GraphNode* node );
    void addEdge( GraphEdge* edge );
    void removeNode( GraphNode* node );
    void removeEdge( GraphEdge* edge );
};

#endif // GRAPH_H
