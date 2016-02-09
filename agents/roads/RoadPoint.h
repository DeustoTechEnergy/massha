#ifndef ROADPOINT_H
#define ROADPOINT_H

#include "agents/Agent.h"

/**
 * @brief The RoadPoint class to represent intersections or road points in the Environment and give them a GraphNode.
 * A road will need the start and end road points to be created.
 */
class RoadPoint : public Agent
{
    Q_OBJECT
public:
    RoadPoint( QString class_name = "RoadPoint" );

};

#endif // ROADPOINT_H
