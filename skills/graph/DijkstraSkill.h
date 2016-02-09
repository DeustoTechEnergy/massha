#ifndef DIJKSTRASKILL_H
#define DIJKSTRASKILL_H

#include "skills/graph/GraphSkill.h"

#include <lemon/dijkstra.h>

#include "utils/graph/GraphEdge.h"
#include "utils/graph/GraphNode.h"

using namespace lemon;

class DijkstraSkill : public GraphSkill
{
    Q_OBJECT
public:
    DijkstraSkill();
    ~DijkstraSkill();

    // METHODS
    QList<GraphEdge*> dijkstraShortestPath(GraphNode* from, GraphNode* to, QList<GraphEdge*> try_to_avoid_roads = QList<GraphEdge*>() , QList<GraphEdge*> do_not_use_roads = QList<GraphEdge*>());
    QList<QList<GraphEdge*> > dijkstraShortestPath(QList<GraphNode*> ordered_nodes, QList<GraphEdge*> try_to_avoid_roads = QList<GraphEdge*>() , QList<GraphEdge*> do_not_use_roads = QList<GraphEdge*>());
    QList<QList<GraphEdge*> > dijkstraShortestPaths(GraphNode* from_node, QList<GraphNode*> to_nodes, QList<GraphEdge*> try_to_avoid_roads = QList<GraphEdge*>() , QList<GraphEdge*> do_not_use_roads = QList<GraphEdge*>());
    GraphNode* dijkstraNearestNode(GraphNode* from_node, QList<GraphNode*> to_nodes, QList<GraphEdge*> try_to_avoid_roads = QList<GraphEdge*>() , QList<GraphEdge*> do_not_use_roads = QList<GraphEdge*>());

};

#endif // DIJKSTRASKILL_H
