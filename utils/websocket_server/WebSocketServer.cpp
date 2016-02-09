#include <QtCore/QDebug>

#include "WebSocketServer.h"


QList<QWebSocket*> WebSocketServer::connected_clients = QList<QWebSocket*>();
QWebSocketServer* WebSocketServer::server = new QWebSocketServer(QStringLiteral("Echo Server"), QWebSocketServer::NonSecureMode);

/**
 * @brief WebSocketServer::getInstance
 * @return
 */
WebSocketServer* WebSocketServer::getInstance(){
    static WebSocketServer instance;
    instance.thread()->setPriority( QThread::TimeCriticalPriority );
    return &instance;
}


WebSocketServer::WebSocketServer(QObject* parent) : QObject(parent)
{
    int WS_PORT = 3001;
    if (WebSocketServer::server->listen(QHostAddress::Any, WS_PORT)) {
        qDebug() << "[WebSocketServer::WebSocketServer] Web Socket Server initiated in port " << WS_PORT << endl;
        QObject::connect(WebSocketServer::server, SIGNAL(newConnection()), this, SLOT(socketConnected()));
        QObject::connect(WebSocketServer::server, SIGNAL(closed()), this, SLOT(socketDisconnected()));
    }
}

WebSocketServer::~WebSocketServer(){
    this->server->close();
    qDeleteAll( WebSocketServer::connected_clients.begin(), WebSocketServer::connected_clients.end() );
}

/**********************************************************************
 SLOTS
**********************************************************************/

void WebSocketServer::socketConnected(){
    QWebSocket *socket = WebSocketServer::server->nextPendingConnection();
    if( !WebSocketServer::connected_clients.contains( socket ) ){
        qDebug() << "[WebSocketServer::socketConnected] New socket connected : " << socket->peerAddress().toString() << endl;
        WebSocketServer::connected_clients << socket;
    }
}

void WebSocketServer::socketDisconnected(){
    QWebSocket *client = qobject_cast<QWebSocket *>( sender() );
    if (client) {
        qDebug() << "[WebSocketServer::socketDisconnected] Socket disconnected : " << client->peerAddress().toString() << endl;
        WebSocketServer::connected_clients.removeAll( client );
        client->deleteLater();
    }
}

void WebSocketServer::push(QByteArray bytes){
    // Send to all connected clients
    foreach(QWebSocket* client , WebSocketServer::connected_clients){
        try{
            client->sendTextMessage( QString(bytes) );
        } catch(...){}
    }
}
