#ifndef ROUTEFOLLOWINGPERSON_H
#define ROUTEFOLLOWINGPERSON_H

#include "agents/Agent.h"
#include "agents/roads/Road.h"
#include "skills/graph/DijkstraSkill.h"

class MovingAgent : public Agent
{
    Q_OBJECT
public:
    MovingAgent(QString class_name = "MovingAgent");
    ~MovingAgent();

    // GETTERS
    GraphEdge* getCurrentEdge();
    double getMovingSpeed();

    // SETTERS
    void setFromAndTo(GraphNode* moving_from , GraphNode* moving_to);
    void setMovingSpeed(double speed); // In m/s

protected slots:
    // Start the agents behaviour
    virtual void agentStarted();
    virtual void movingStarted();
    virtual void moveToNextEdge();
    virtual void movingEnded();
    virtual void agentEnded();

protected:

    // METHODS
    virtual QList<GraphEdge*> calculateRoute(GraphNode* from , GraphNode* to );

    // Node from and to
    GraphNode* moving_from;
    GraphNode* moving_to;
    double moving_speed;

    // Route
    QList<GraphEdge*> moving_route;
    GraphEdge* moving_current_edge;
};

#endif // ROUTEFOLLOWINGPERSON_H
