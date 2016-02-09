#ifndef AGENTTOUI_H
#define AGENTTOUI_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QByteArray>

#include "agents/Agent.h"

QT_FORWARD_DECLARE_CLASS(Agent)

class AgentToUI : public QObject
{
    Q_OBJECT
public:
    enum operations {
        CREATE = 0,
        UPDATE = 1,
        DELETE = 2
    };

    static QByteArray formatAgent( Agent* agent, AgentToUI::operations operation );
    static QByteArray formatAgent( QList<Agent*> agents, AgentToUI::operations operation );
    static QJsonObject formatProperty( QVariant value , QString post = "" , QString pre = "" );
    static QJsonObject formatProperty( Agent* agent , QString post = "" , QString pre = "" );

};

Q_DECLARE_METATYPE(AgentToUI::operations)

#endif // AGENTTOUI_H
