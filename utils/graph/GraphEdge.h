#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H

#include "geos/planargraph/DirectedEdge.h"
#include "geos/planargraph/PlanarGraph.h"

#include "utils/graph/GraphNode.h"

QT_FORWARD_DECLARE_CLASS(GraphNode)

using namespace geos::geom;
using namespace geos::planargraph;

class GraphEdge : public geos::planargraph::DirectedEdge
{
public:
    GraphEdge(GraphNode* start_node, GraphNode* end_node);

    // GETTERS
    QString objectName();
    GraphNode* getStartNode();
    GraphNode* getEndNode();
    virtual double getCost();
    virtual double getLength();

    // SETTERS
    void setObjectName(QString name);
    virtual void setCost(double cost);
    virtual void setLength(double length);

    // METHODS

private:
    QString object_name;
    double cost;
    double length; // In meters
};

#endif // GRAPHEDGE_H
