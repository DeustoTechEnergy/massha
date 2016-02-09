#ifndef GRAPHSKILL_H
#define GRAPHSKILL_H

#include "skills/Skill.h"

#include <lemon/core.h>
#include <lemon/list_graph.h>

#include <geos/geom/Geometry.h>
#include <geos/geom/Coordinate.h>

#include "utils/graph/GraphEdge.h"
#include "utils/graph/GraphNode.h"

using namespace geos::geom;
using namespace lemon;

class GraphSkill : public Skill
{
    Q_OBJECT

public:

    // METHODS
    void createGraph(QMap<GraphEdge*,double> edges);

protected:
    GraphSkill(); // Only children can create a GraphSkill
    ~GraphSkill();

    // Graph
    ListDigraph* network_graph;
    // Costs
    ListDigraph::ArcMap<double>* network_graph_weights;
    // Node coordinate map to avoid duplicates
    QMap<geos::geom::Coordinate, ListDigraph::Node> node_coordinates;
    // Node and Edge arc map to retrieve them
    QMap<ListDigraph::Arc, GraphEdge*> arcs;
    QMap<ListDigraph::Node, GraphNode*> nodes;

};

#endif // GRAPHSKILL_H
