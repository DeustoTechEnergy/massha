#ifndef TSPSKILL_H
#define TSPSKILL_H

#include "skills/graph/GraphSkill.h"

#include <lemon/core.h>
#include <lemon/list_graph.h>
#include <lemon/full_graph.h>

#include "utils/graph/GraphEdge.h"
#include "utils/graph/GraphNode.h"

/**
 * @brief The TspSkill class
 * Requires two graphs, first one (inherited from GraphSkill) is the network and
 * another created in each method which represents the distance-matrix from each
 * stop to all the others.
 */
class TspSkill : public GraphSkill
{
public:
    TspSkill();
    ~TspSkill();

    // METHODS
    QList<GraphNode*> nearestNeighborTsp(GraphNode* start_node, GraphNode* end_node, QList<GraphNode*> go_through_nodes);
    QList<GraphNode*> greedyTsp(GraphNode* start_node, GraphNode* end_node, QList<GraphNode*> go_through_nodes);
    QList<GraphNode*> insertionTsp(GraphNode* start_node, GraphNode* end_node, QList<GraphNode*> go_through_nodes);
    QList<GraphNode*> christofidesTsp(GraphNode* start_node, GraphNode* end_node, QList<GraphNode*> go_through_nodes);
    QList<GraphNode*> opt2Tsp(GraphNode* start_node, GraphNode* end_node, QList<GraphNode*> go_through_nodes);

private:
    // Cost matrix as graph
    FullGraph* distance_matrix;
    // Distance matrix costs
    FullGraph::EdgeMap<double>* distance_matrix_weights;

    // Map to relate graph and distance matrix elements
    QMap<ListDigraph::Node , FullGraph::Node> graph_matrix_nodes;

    bool createDistanceMatrix(QList<GraphNode*> go_through_nodes);
    QList<GraphNode*> orderCircularTsp(GraphNode* start_node, GraphNode* end_node, QList<GraphNode*> tsp_circular_nodes);

};

#endif // TSPSKILL_H
