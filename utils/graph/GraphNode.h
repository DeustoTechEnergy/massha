#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <QString>
#include <QList>
#include "geos/geom/Coordinate.h"
#include "geos/planargraph/Node.h"
#include "geos/planargraph/PlanarGraph.h"

#include "utils/graph/GraphEdge.h"

QT_FORWARD_DECLARE_CLASS(GraphEdge)

using namespace geos::geom;
using namespace geos::planargraph;

class GraphNode : public geos::planargraph::Node
{
public:
    GraphNode( Coordinate coor );
    ~GraphNode();

    // GETTERS
    QString objectName();
    QList<GraphEdge*> getDepartingEdges();

    // SETTERS
    void setObjectName(QString name);
    void connect(GraphEdge* edge);
    void disconnect(GraphEdge* edge);

private:
    QString object_name;
};

#endif // GRAPHNODE_H
