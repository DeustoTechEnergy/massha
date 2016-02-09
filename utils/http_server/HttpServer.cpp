#include <qhttpserver.h>
#include <QThread>

#include "HttpServer.h"

HttpServer* HttpServer::getInstance(){
    static HttpServer instance;
    instance.thread()->setPriority( QThread::TimeCriticalPriority );
    return &instance;
}

HttpServer::HttpServer()
{
    this->handler = new RequestHandler();

    this->url_creator = new RequestProcessorCreator(this->handler);
    QThread url_creator_thread;
    this->url_creator->moveToThread( &url_creator_thread );

    this->server = new QHttpServer(this);
    int HTTP_PORT = 3000;

    if (this->server->listen(QHostAddress::Any, HTTP_PORT)){

        // Connect to qHttpServer signal
        connect(this->server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)), this->handler, SLOT(handleHttpRequest(QHttpRequest*, QHttpResponse*)));

        qDebug() << "[HttpServer::HttpServer] HttpServer initiated in port " << HTTP_PORT << endl;
    } else {
        qWarning() << "[HttpServer::HttpServer] HttpServer could not be initiated in port " << HTTP_PORT << endl;
    }
}

HttpServer::~HttpServer(){
    delete this->handler;
    delete this->server;
    delete this->url_creator;
}

void HttpServer::createUrls(QString class_name){
    this->url_creator->createUrlsForAgents(this->handler, class_name);
}
