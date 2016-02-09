#ifndef ROAD_H
#define ROAD_H

#include "agents/roads/RoadConstants.h"
#include "skills/contain/AgentContainSkill.h"
#include "utils/graph/GraphEdge.h"

QT_FORWARD_DECLARE_CLASS(Vehicle)

class Road : public Agent, public GraphEdge
{
        Q_OBJECT
public:
    Road(RoadConstants::road_types type, GraphNode* start_node, GraphNode* end_node, QString class_name = "Road");
    ~Road();
    virtual QJsonObject toJSON();
    virtual void fromJSON(QJsonObject json);

    // GETTERS
    RoadConstants::road_types getRoadType();
    double getGradient(); // Positive for climbing up and negative for going down
    int getLaneAmount();
    double getMaxSpeed();
    double getLength();
    bool isClosed();
    virtual double getCost();

    // SETTERS
    virtual void setGeometry(Geometry* geometry);
    void setRoadType(RoadConstants::road_types road_type);
    void setLaneAmount(int agent_amount);
    void setLength(double length);
    void setMaxSpeed(double max_speed);
    void setClosed(bool is_closed);

    // METHODS

private:

    RoadConstants::road_types road_type; // Road type
    double max_speed; // In meter/second
    double length; // In meters
    bool is_closed; // Closed

};

#endif // ROAD_H
