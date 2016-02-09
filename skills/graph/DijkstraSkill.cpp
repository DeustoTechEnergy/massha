#include "DijkstraSkill.h"

#include <QDebug>

#include <lemon/bellman_ford.h>
#include <lemon/path.h>

DijkstraSkill::DijkstraSkill() : GraphSkill(){
}

DijkstraSkill::~DijkstraSkill(){
}

/**********************************************************************
 METHODS
**********************************************************************/

/**
 * @brief DijkstraSkill::dijkstraShortestPath Given the ordered nodes to go through, calculates the entire route
 * @param edges
 * @param go_through_nodes
 * @return
 */
QList<QList<GraphEdge*> > DijkstraSkill::dijkstraShortestPath(QList<GraphNode*> ordered_nodes, QList<GraphEdge*> try_to_avoid_roads , QList<GraphEdge*> do_not_use_roads){
    QList<QList<GraphEdge*> > result_routes;

    // IF TRY_TO_AVOID_ROADS
    // Avoid roads will be given a 99999999 cost in the graph
    // To restore its original value, store it in this hashmap
    QMap<GraphEdge* , double> try_to_avoid_roads_real_cost;
    foreach(GraphEdge* road , try_to_avoid_roads){
        if( this->arcs.values().contains( road ) ){
            ListDigraph::Arc arc = this->arcs.key( road );
            double value = this->network_graph_weights->operator [](arc);
            try_to_avoid_roads_real_cost.insert( road , value );
            this->network_graph_weights->set( arc , 9999999999 );
        }
    }

    // IF DO_NOT_USE_ROADS
    // Do not use roads will be removed from the graph
    // To restore its original value, store it in this hashmap
    /*QMap<GraphEdge* , double> avoid_roads_real_cost;
    foreach(GraphEdge* road , avoid_roads){
        if( this->arcs.values().contains( road ) ){
            ListDigraph::Arc arc = this->arcs.key( road );
            double value = this->network_graph_weights->operator [](arc);
            avoid_roads_real_cost.insert( road , value );
            this->network_graph_weights->erase( arc );
        }
    }*/

    Dijkstra<ListDigraph, ListDigraph::ArcMap<double> > dijkstra_graph( *this->network_graph , *this->network_graph_weights );

    for(int i = 0; i < ordered_nodes.size()-1; i++){

        QList<GraphEdge*> result_route;
        GraphNode* from_node = ordered_nodes.at( i );
        GraphNode* to_node = ordered_nodes.at( i+1 );

        if( !from_node || !to_node ){
            qWarning() << "[DijkstraSkill:dijkstraShortestPath] Start (" << from_node->objectName() << ") or end node (" << to_node->objectName() << ") are not in graph, route part will be discarded." << endl;
            result_routes.append( result_route );
            continue;
        }

        if( from_node->getCoordinate().equals( to_node->getCoordinate() ) ){
            qWarning() << "[DijkstraSkill:dijkstraShortestPath] Same start and end nodes (" << from_node->objectName() << "), route part will be discarded." << endl;
            result_routes.append( result_route );
            continue;
        }

        Coordinate start_node_coor = from_node->getCoordinate();
        Coordinate end_node_coor = to_node->getCoordinate();

        if ( !this->node_coordinates.contains( start_node_coor ) || !this->node_coordinates.contains( end_node_coor ) ){
            qWarning() << "[DijkstraSkill:dijkstraShortestPath] Start (" << from_node->objectName() << ") or end node (" << to_node->objectName() << ") are not in graph, route part will be discarded." << endl;
            result_routes.append( result_route );
            continue;
        }

        // Compute dijkstra shortest path
        ListDigraph::Node start = this->node_coordinates.value( start_node_coor );
        ListDigraph::Node end = this->node_coordinates.value( end_node_coor );

        if( !dijkstra_graph.run( start , end ) ){
            qWarning() << "[DijkstraSkill:dijkstraShortestPath] Can not reach end node (" << to_node->objectName() << ") from start (" << from_node->objectName() << ") , route part will be discarded." << endl;
            result_routes.append( result_route );
            continue;
        }

        // Get route
        Path<ListDigraph> shortest_path = dijkstra_graph.path( end );
        for(int i = 0 ; i < shortest_path.length() ; i++) {
            ListDigraph::Arc arc = shortest_path.nth( i );
            result_route.append( this->arcs.value( arc ) );
        }
        result_routes.append( result_route );
    }


    // IF TRY_TO_AVOID_ROADS
    // Restore the avoid_roads in graph to their original cost
    foreach(GraphEdge* road , try_to_avoid_roads_real_cost.keys()){
        if( this->arcs.values().contains( road ) ){
            ListDigraph::Arc arc = this->arcs.key( road );
            this->network_graph_weights->set( arc , try_to_avoid_roads_real_cost.value( road ) );
        }
    }

    // IF AVOID_ROADS
    // Restore the avoid_roads in graph to their original cost
    /*foreach(GraphEdge* road , avoid_roads_real_cost.keys()){
        ListDigraph::Arc arc = this->arcs.key( road );
        this->network_graph_weights->set( arc , avoid_roads_real_cost.value( road ) );
    }*/

    return result_routes;
}

/**
 * @brief DijkstraSkill::dijkstraShortestPath Given a star and a list of end nodes calculates all the routes
 * @param edges
 * @param start_node
 * @param end_nodes
 * @return
 */
QList<QList<GraphEdge*> > DijkstraSkill::dijkstraShortestPaths(GraphNode *from_node, QList<GraphNode *> to_nodes, QList<GraphEdge*> try_to_avoid_roads , QList<GraphEdge*> do_no_use_roads){
    QList<QList<GraphEdge*> > result_routes;

    // Get start node and start graph from it
    Coordinate start_node_coor = from_node->getCoordinate();
    ListDigraph::Node start = this->node_coordinates.value( start_node_coor );

    Dijkstra<ListDigraph, ListDigraph::ArcMap<double> > dijkstra_graph( *this->network_graph , *this->network_graph_weights );
    dijkstra_graph.run( start );

    // Iterate all end nodes
    foreach( GraphNode* node , to_nodes ){
        QList<GraphEdge*> route;

        Coordinate node_coor = node->getCoordinate();
        ListDigraph::Node n = this->node_coordinates.value( node_coor );

        Path<ListDigraph> shortest_path = dijkstra_graph.path( n );
        for(int i = 0 ; i < shortest_path.length() ; i++) {
            ListDigraph::Arc arc = shortest_path.nth( i );
            route.append( this->arcs.value( arc ) );
        }

        result_routes.append( route );
    }

    return result_routes;
}

/**
 * @brief DijkstraSkill::dijkstraShortestPath Given a start and end node, calculates the route
 * @param edges
 * @param start_node
 * @param end_node
 * @return
 */
QList<GraphEdge*> DijkstraSkill::dijkstraShortestPath(GraphNode* from_node, GraphNode* to_node, QList<GraphEdge*> try_to_avoid_roads , QList<GraphEdge*> do_not_use_roads){

    QList<GraphNode*> nodes;
    nodes.append( from_node );
    nodes.append( to_node );

    return this->dijkstraShortestPath(nodes , try_to_avoid_roads).at(0);
}

/**
 * @brief DijkstraSkill::dijkstraNearestNode Given a set of GraphNodes, calculates the routing nearest one
 * @param edges Edges to create the graph
 * @param nodes
 * @return
 */
GraphNode* DijkstraSkill::dijkstraNearestNode(GraphNode* from_node, QList<GraphNode*> to_nodes, QList<GraphEdge*> try_to_avoid_roads , QList<GraphEdge*> do_no_use_roads){

    GraphNode* result_node = 0;
    double min_cost = std::numeric_limits<double>::max();

    QList<QList<GraphEdge*> > routes = this->dijkstraShortestPaths(from_node , to_nodes , try_to_avoid_roads);

    if( routes.size() != to_nodes.size() ){
        qWarning() << "[DijkstraSkill::dijkstraNearestNode] Node list and routes size do not match";
    }

    for(int node_pos = 0; node_pos < routes.size() && node_pos < to_nodes.size() ; node_pos++){
        double c = 0;

        foreach( GraphEdge* edge , routes.at(node_pos) ){
            c += edge->getCost();
        }

        if( c <= min_cost ){
            min_cost = c;
            result_node = to_nodes.at( node_pos );
        }
    }

    return result_node;
}
