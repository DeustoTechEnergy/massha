#ifndef HTTPSERVER_H
#define HTTPSERVER_H

// QHTTPSERVER
// https://github.com/nikhilm/qhttpserver
#include <qhttpserver.h>
#include <qhttprequest.h>
#include <qhttpresponse.h>

#include "utils/http_server/RequestProcessorCreator.h"

class HttpServer : public QObject
{
    Q_OBJECT
public:

    static HttpServer* getInstance();

public slots:
    void createUrls(QString class_name);

private:
    HttpServer();
    HttpServer(HttpServer const&);
    ~HttpServer();

    QHttpServer* server;
    RequestProcessorCreator* url_creator;
    RequestHandler* handler;
};

#endif // HTTPSERVER_H
