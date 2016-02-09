#include "AgentToUI.h"
#include <QJsonDocument>
#include <QVariantMap>
#include "utils/websocket_server/WebSocketServer.h"

/**
 * @brief AgentToUI::toUI UI expects messages with format:
 *
 * {
 *  data : Agent
 *  operation : operation_code
 * }
 */
QByteArray AgentToUI::formatAgent(Agent* agent, AgentToUI::operations operation){
    QVariantMap json_elements_and_type;
    json_elements_and_type.insert("operation", operation);
    json_elements_and_type.insert("data", agent->toJSON().toVariantMap());
    return QJsonDocument::fromVariant( json_elements_and_type ).toJson();
}

/**
 * @brief AgentToUI::toUI UI expects messages with format:
 *
 * {
 *  data : [Agents]
 *  operation : operation_code
 * }
 */
QByteArray AgentToUI::formatAgent(QList<Agent*> agents, AgentToUI::operations operation){
    QVariantMap json_elements_and_type;
    json_elements_and_type.insert("operation", operation);
    QJsonArray json_agents;
    foreach(Agent* a , agents){
        json_agents.append( a->toMiniJSON() );
    }
    json_elements_and_type.insert("data", json_agents.toVariantList());
    return QJsonDocument::fromVariant( json_elements_and_type ).toJson();
}


/**
 * @brief AgentToUI::property Creates the JSON format for correct displaying properties in the UI:
 *
 * properties {
 *  prop1 : {type : 'text' , value : 'hello' , pre : 'I say'}
 *  prop2 : {...}
 * }
 *
 * @param value Property value
 * @param pre Text to be written before the property
 * @param post Text to be written after the property
 * @return
 */
QJsonObject AgentToUI::formatProperty(QVariant value, QString post, QString pre ){
    QJsonObject json;
    switch( value.type() ){
    case QVariant::String:
        json.insert("type" , "text");
        json.insert("value" , value.toString() );
        break;
    case QVariant::Int:
    case QVariant::UInt:
    case QVariant::Double:
    case QVariant::LongLong:
        json.insert("value" , value.toDouble() );
        json.insert("type" , "number");
        break;
    case QVariant::Bool:
        json.insert("value" , value.toBool() );
        json.insert("type" , "checkbox");
        break;
    case QVariant::Color:
        json.insert("value" , value.toString() );
        json.insert("type" , "color");
        break;
    case QVariant::DateTime:
    case QVariant::Date:
        json.insert("value" , value.toDateTime().toMSecsSinceEpoch() );
        json.insert("type" , "datetime-local");
        break;
    default:
        qWarning() << "[AgentToUI::property] Unknown type" << QVariant::typeToName( value.type() );
    }
    if( !pre.isEmpty() ){ json.insert("pre" , pre); }
    if( !post.isEmpty() ){ json.insert("post", post); }
    return json;
}

/**
 * @brief AgentToUI::property Creates the JSON format for correct displaying properties in the UI:
 *
 * properties {
 *  prop1 : {type : 'agent' , class : 'XX' , id : 'XX'}
 *  prop2 : {...}
 * }
 *
 * @param value Property value
 * @param pre Text to be written before the property
 * @param post Text to be written after the property
 * @return
 */
QJsonObject AgentToUI::formatProperty(Agent* agent, QString post, QString pre ){
    QJsonObject json;
    json.insert("type" , "agent");
    if( !pre.isEmpty() ){ json.insert("pre" , pre); }
    if( !post.isEmpty() ){ json.insert("post", post); }
    if( agent ){
        json.insert("id" , ((int) agent->getId()) );
        json.insert("class" , agent->getClassName() );
        json.insert("name" , agent->objectName() );
    }
    return json;
}
