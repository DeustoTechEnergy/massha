#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QtWebSockets>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtCore/QByteArray>

class WebSocketServer : public QObject
{
    Q_OBJECT
public:
    static WebSocketServer* getInstance();
    ~WebSocketServer();

public slots:
    void push(QByteArray bytes);

private slots:
    void socketConnected();
    void socketDisconnected();

private:
    WebSocketServer(QObject* parent = 0);
    WebSocketServer(WebSocketServer const&);

    static QWebSocketServer* server;
    static QList<QWebSocket*> connected_clients;
};

#endif // WEBSOCKETSERVER_H
