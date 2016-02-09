#include "GraphSkill.h"

GraphSkill::GraphSkill() : Skill(){
    this->network_graph = 0;
    this->network_graph_weights = 0;
}

GraphSkill::~GraphSkill(){
    // Delete created stuff
    if( this->network_graph ) { delete this->network_graph; }
    if( this->network_graph_weights ) { delete this->network_graph_weights; }
}

/**********************************************************************
 METHODS
**********************************************************************/

void GraphSkill::createGraph(QMap<GraphEdge* , double> edges){

    if( !edges.size() ){
        qWarning() << QString("[DijkstraSkill::createGraph] No edges given to create graph.") << endl;
        return;
    }

    // Create graph and edge weights map
    this->network_graph = new ListDigraph();
    this->network_graph_weights = new ListDigraph::ArcMap<double>( *this->network_graph );

    // Create nodes and edges
    foreach(GraphEdge* edge, edges.keys()){

        try{
            // Create or retrieve edge start node
            Coordinate start_node_coor = edge->getStartNode()->getCoordinate();
            ListDigraph::Node s;
            if( !this->node_coordinates.contains( start_node_coor ) ){
                s = this->network_graph->addNode();
                this->node_coordinates.insert( start_node_coor , s );
                this->nodes.insert( s , edge->getStartNode() );
            } else {
                s = this->node_coordinates.value( start_node_coor );
            }

            // Create or retrieve edge end node
            Coordinate end_node_coor = edge->getEndNode()->getCoordinate();
            ListDigraph::Node e;
            if( !this->node_coordinates.contains( end_node_coor ) ){
                e = this->network_graph->addNode();
                this->node_coordinates.insert( end_node_coor , e );
                this->nodes.insert( e , edge->getEndNode() );
            } else {
                e = this->node_coordinates.value( end_node_coor );
            }

            // Create edge and its cost
            ListDigraph::Arc arc = this->network_graph->addArc(s , e);
            this->arcs.insert( arc , edge );
            this->network_graph_weights->set( arc , edges.value( edge ) );

        } catch(...){}
    }
}
