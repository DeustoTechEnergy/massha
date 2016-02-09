#ifndef AGENT_H
#define AGENT_H

#include <QObject>
#include <QMetaType>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QThread>
#include <QDebug>
#include <QTimer> // QTIMER::SINGLESHOT queues a new slot in the event loop to be executed in a certain time

#include "geos/geom/Geometry.h"
#include "geos/geom/Point.h"
#include "geos/geom/LineString.h"
#include "geos/geom/Polygon.h"

#include "utils/graph/GraphNode.h"

#include "skills/SkillConstants.h"
#include "utils/uistyle/UiStyle.h"
#include "utils/exporters/agent_to_ui/AgentToUI.h"

QT_FORWARD_DECLARE_CLASS(Message)

using namespace geos::geom;
using namespace geos::planargraph;

class Agent : public QObject
{
    friend class Environment; // Environment will be able to overwrite the agents attributes (geometry and graphnode)
    Q_OBJECT // Required for every child!! Adds Qt extra functionality like SIGNAL/SLOTS
public:
    ~Agent();
    QJsonObject toMiniJSON(); // Called when asked for many agents.
    virtual QJsonObject toJSON(); // Called when asked for more info about this agent. Can be overwritten if want to add extra variables to the exporters
    virtual void fromJSON(QJsonObject json); // Update agent according to parameters sent from importers

    // SETTERS
    void setGeometry(geos::geom::Geometry* geometry);
    void setGraphNode(GraphNode* node);
    void setBornDatetime(QDateTime born_datetime);
    void setStyle( UiStyle* style );

    // GETTERS
    unsigned int getId();
    QString getClassName();
    geos::geom::Geometry* getGeometry();
    GraphNode* getGraphNode();
    QList<SkillConstants::capabilities> getCapabilities();
    bool isRunning();
    QDateTime getBornDatetime();
    QTimer* getTimer();
    UiStyle* getStyle();

signals:
    void updateUISignal(QByteArray bytes);
    void agentStartedSignal();
    void agentEndedSignal();

public slots:
    // Start or Stop the agents behaviour in a new thread
    // This methods only needs to be in Agent
    void run();
    void stop();

protected slots:
    // To be implemented by children when the agents behaviour starts and ends
    virtual void agentStarted(); // Called by run() method
    virtual void agentEnded(); // Called by agent when task is finished

protected:
    // Abstract agent, cant be created
    explicit Agent(QString class_name = "Agent", QObject* parent = 0);

    // FLAGS
    bool running;

private:

    // AUTOINCREMENTAL FOR IDS
    static unsigned int counter;

    // ALL AGENTS COMMON VARIABLES
    /**
     * @brief id
     */
    const unsigned int id; // Agent id

    /**
     * @brief born_datetime Agent born/creation datetime
     */
    QDateTime born_datetime; // Entity creation/borning datetime

    /**
     * @brief class_name AGENT TYPE ( CLASS_NAME ) in order to group them both in the environment
     * and in the user interface. All agents within the same group will be stored together and
     * the environment will have methods to search for one specific TYPE.
     *
     * So each Agent child implementation should overwrite its CLASS_NAME in order to be grouped with its
     * types. If a specific type of agent needs to be grouped with some other types, the CLASS_NAME
     * can be set to the same as the other.
     *
     */
    const QString class_name; // Agent class_name

    /**
     * @brief Agent's geometry
     */
    geos::geom::Geometry* geometry; // Agents geometry

    /**
     * @brief Agent's node on environment graph
     */
    GraphNode *node;

    /**
     * Color, icon style...
     **/
    UiStyle* style;

    QTimer* timer; // For slot invoking
    QList<SkillConstants::capabilities> capabilities;

};

#endif // AGENT_H
