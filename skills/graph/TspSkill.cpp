#include "TspSkill.h"
#include "lemon/dijkstra.h"
#include "lemon/nearest_neighbor_tsp.h"
#include "lemon/christofides_tsp.h"
#include "lemon/greedy_tsp.h"
#include "lemon/insertion_tsp.h"
#include "lemon/opt2_tsp.h"

TspSkill::TspSkill() : GraphSkill(){
    this->distance_matrix = 0;
    this->distance_matrix_weights = 0;
}

TspSkill::~TspSkill() {
    // Delete created stuff
    if( this->distance_matrix ) { delete this->distance_matrix; }
    if( this->distance_matrix_weights ) { delete this->distance_matrix_weights; }
}

/**********************************************************************
 METHODS
**********************************************************************/

QList<GraphNode*> TspSkill::nearestNeighborTsp(GraphNode* start_node, GraphNode* end_node, QList<GraphNode*> go_through_nodes){

    if( !createDistanceMatrix(go_through_nodes) ){
        return QList<GraphNode*>();
    }

    NearestNeighborTsp< FullGraph::EdgeMap<double> >* tsp = new NearestNeighborTsp< FullGraph::EdgeMap<double> >( *this->distance_matrix , *this->distance_matrix_weights );
    tsp->run();
    QList<GraphNode*> tsp_circular_route_nodes;
    foreach( FullGraph::Node n , tsp->tourNodes() ){
        tsp_circular_route_nodes.append( this->nodes.value( this->graph_matrix_nodes.key( n ) ) );
    }

    return this->orderCircularTsp(start_node , end_node , tsp_circular_route_nodes);
}

QList<GraphNode*> TspSkill::greedyTsp(GraphNode* start_node, GraphNode* end_node, QList<GraphNode*> go_through_nodes){

    if( !createDistanceMatrix(go_through_nodes) ){
        return QList<GraphNode*>();
    }

    GreedyTsp< FullGraph::EdgeMap<double> >* tsp = new GreedyTsp< FullGraph::EdgeMap<double> >( *this->distance_matrix , *this->distance_matrix_weights );
    tsp->run();
    QList<GraphNode*> tsp_circular_route_nodes;
    foreach( FullGraph::Node n , tsp->tourNodes() ){
        tsp_circular_route_nodes.append( this->nodes.value( this->graph_matrix_nodes.key( n ) ) );
    }

    return this->orderCircularTsp(start_node , end_node , tsp_circular_route_nodes);
}

QList<GraphNode*> TspSkill::insertionTsp(GraphNode* start_node, GraphNode* end_node, QList<GraphNode*> go_through_nodes){

    if( !createDistanceMatrix(go_through_nodes) ){
        return QList<GraphNode*>();
    }

    InsertionTsp< FullGraph::EdgeMap<double> >* tsp = new InsertionTsp< FullGraph::EdgeMap<double> >( *this->distance_matrix , *this->distance_matrix_weights );
    tsp->run();
    QList<GraphNode*> tsp_circular_route_nodes;
    foreach( FullGraph::Node n , tsp->tourNodes() ){
        tsp_circular_route_nodes.append( this->nodes.value( this->graph_matrix_nodes.key( n ) ) );
    }

    return this->orderCircularTsp(start_node , end_node , tsp_circular_route_nodes);
}

QList<GraphNode*> TspSkill::christofidesTsp(GraphNode* start_node, GraphNode* end_node, QList<GraphNode*> go_through_nodes){

    if( !createDistanceMatrix(go_through_nodes) ){
        return QList<GraphNode*>();
    }

    InsertionTsp< FullGraph::EdgeMap<double> >* tsp = new InsertionTsp< FullGraph::EdgeMap<double> >( *this->distance_matrix , *this->distance_matrix_weights );
    tsp->run();
    QList<GraphNode*> tsp_circular_route_nodes;
    foreach( FullGraph::Node n , tsp->tourNodes() ){
        tsp_circular_route_nodes.append( this->nodes.value( this->graph_matrix_nodes.key( n ) ) );
    }

    return this->orderCircularTsp(start_node , end_node , tsp_circular_route_nodes);
}

QList<GraphNode*> TspSkill::opt2Tsp(GraphNode* start_node, GraphNode* end_node, QList<GraphNode*> go_through_nodes){

    if( !createDistanceMatrix(go_through_nodes) ){
        return QList<GraphNode*>();
    }

    InsertionTsp< FullGraph::EdgeMap<double> >* tsp = new InsertionTsp< FullGraph::EdgeMap<double> >( *this->distance_matrix , *this->distance_matrix_weights );
    tsp->run();
    QList<GraphNode*> tsp_circular_route_nodes;
    foreach( FullGraph::Node n , tsp->tourNodes() ){
        tsp_circular_route_nodes.append( this->nodes.value( this->graph_matrix_nodes.key( n ) ) );
    }

    return this->orderCircularTsp(start_node , end_node , tsp_circular_route_nodes);
}

/**********************************************************************
 PRIVATE
**********************************************************************/

/**
 * @brief TspSkill::createDistanceMatrix Inits the distance matrix and costs for all TSP algorithms
 * @param start_node
 * @param end_node
 * @param go_through_nodes
 * @return
 */
bool TspSkill::createDistanceMatrix(QList<GraphNode *> go_through_nodes){

    // List of nodes in the graph
    QList< ListDigraph::Node > travel_graph_nodes;

    foreach(GraphNode* node , go_through_nodes){
        Coordinate node_coor = node->getCoordinate();

        if ( this->node_coordinates.contains( node_coor ) ){
            travel_graph_nodes.append( this->node_coordinates.value( node_coor ) );
        } else {
            qWarning() << QString("[TspSkill:nearestNeighborTsp] One node is not in graph, it will be discarded.") << endl;
        }
    }

    // Create distance_matrix (it automatically creates nodes and edges with ids from 0)
    this->distance_matrix = new FullGraph( travel_graph_nodes.size() );
    // Create costmap
    this->distance_matrix_weights = new FullGraph::EdgeMap<double>( *this->distance_matrix );

    // Iterate distance_matrix nodes and link them to network nodes
    for(int i = 0; i < this->distance_matrix->nodeNum(); i++){
        this->graph_matrix_nodes.insert( travel_graph_nodes.at( i ) , this->distance_matrix->nodeFromId(i) );
    }

    // Create routing graph for routing distance calculations
    Dijkstra<ListDigraph, ListDigraph::ArcMap<double> > dijkstra_graph( *this->network_graph , *this->network_graph_weights );

    // Iterate all edges in distance matrix and calculate their costs
    for(int i = 0; i < this->graph_matrix_nodes.size(); i++ ){
        for(int j = i+1; j < this->graph_matrix_nodes.size(); j++ ){

                FullGraph::Node from = this->graph_matrix_nodes.values().at(i);
                FullGraph::Node to = this->graph_matrix_nodes.values().at(j);
                FullGraph::Edge edge = this->distance_matrix->edge( from , to );

                // Calculate routing distance
                dijkstra_graph.run( this->graph_matrix_nodes.key( from ) , this->graph_matrix_nodes.key( to ) );
                double distance = dijkstra_graph.dist( this->graph_matrix_nodes.key( to ) );

                this->distance_matrix_weights->set( edge , distance );
        }
    }
    return true;
}


/**
 * @brief TspSkill::orderCircularTsp TSP algorithms return the shortest circular route between all through nodes
 * but don't tell from which node start and end. Order the circular route here
 * @param start_node
 * @param end_node
 * @param tsp_circular_nodes
 * @return
 */
QList<GraphNode*> TspSkill::orderCircularTsp(GraphNode *start_node, GraphNode *end_node, QList<GraphNode*> tsp_circular_nodes){
    Q_UNUSED(end_node)

    QList<GraphNode*> tsp_ordered_nodes;

    // Find the nearest circular_route node to start_node
    GraphNode* nearest = tsp_circular_nodes.at( 0 );
    double min_distance = start_node->getCoordinate().distance( nearest->getCoordinate() );
    foreach(GraphNode* n , tsp_circular_nodes){
        double d = n->getCoordinate().distance( start_node->getCoordinate() );
        if( min_distance > d ){
            nearest = n;
            min_distance = d;
        }
    }

    // Now order the circular route starting from the nearest node
    int nearest_index = tsp_circular_nodes.indexOf( nearest );
    for(int i = 0; i < tsp_circular_nodes.size() ; i++){
        tsp_ordered_nodes.append( tsp_circular_nodes.at( (nearest_index + i) % tsp_circular_nodes.size() ) );
    }

    return tsp_ordered_nodes;
}
