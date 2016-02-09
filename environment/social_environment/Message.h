#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QString>
#include <QDateTime>

#include "agents/Agent.h"
#include "environment/social_environment/MessageConstants.h"

QT_FORWARD_DECLARE_CLASS(Agent)

class Message : public QObject
{
        Q_OBJECT
public:
    Message(QObject *parent = 0);

private:

    MessageConstants::message_types performative;
    int conversation_id;
    Agent* sender;
    QList<Agent*> receivers;
    QString language;
    QString ontology;
    QString content;

};

#endif // MESSAGE_H
