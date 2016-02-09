#include <QDebug>
#include <QFile>
#include "RequestHandler.h"

#include "environment/Environment.h"
#include "agents/Agent.h"
#include "utils/exporters/agent_to_ui/AgentToUI.h"

RequestHandler::RequestHandler() : QObject(){
}

void RequestHandler::handleHttpRequest(QHttpRequest* req, QHttpResponse* res){

    qDebug() << "[HttpHandler::handleHttpRequest] Request entered" << req->method() << req->url();

    // Find if the req matches the environment urls
    bool found = false;
    if ( urls.keys().contains( req->method() ))
    {
        foreach( RequestProcessor* processor , urls.value(req->method()) ){
            QRegularExpressionMatch matchedRegexp = processor->match( req->url() );
            if ( matchedRegexp.hasMatch() && !found ){

                // New request received, but its data wont be ready
                // untill end() signal is called. So store the
                // references in the urlmatch and once the req has
                // ended, the urlmatch will emit signal.
                req->storeBody();

                // Store references and wait for req to end to parse body
                processor->storeReferencesUntillReqEnd(req, res, matchedRegexp);
                connect( req , SIGNAL( end() ), processor , SLOT( requestEnded() ));
                found = true;
            }
        }
    }

    if (!found){
        qWarning() << "[HttpHandler::handleHttpRequest] Unexpected Http request :" << req->methodString() << req->url().path() << endl;
        res->deleteLater();
        req->deleteLater();
    }
}


/**
 * @brief HttpHandler::containsUrlMath
 * @param method
 * @param urlRegex
 * @return
 */
bool RequestHandler::containsUrlMath(QHttpRequest::HttpMethod method, QRegularExpression urlRegex){
    bool contains = false;
    if(urls.keys().contains(method)){
        QVectorIterator<RequestProcessor*> i(urls[method]);
        while (i.hasNext() && !contains){
            contains = urlRegex == i.next()->getUrlRegex();
        }
    }
    return contains;
}

/**
 * @brief HttpHandler::addUrlMatch
 * @param method
 * @param urlRegex
 */
RequestProcessor* RequestHandler::addUrlMatch(QHttpRequest::HttpMethod method, QRegularExpression urlRegex){
    if ( !this->containsUrlMath(method, urlRegex) ){
        RequestProcessor *processor = new RequestProcessor(method, urlRegex, this);
        urls[method].append(processor);
        return processor;
    }
    return 0;
}

/**********************************************************************
 SLOTS
**********************************************************************/

// Slots called by RequestProcessors
void RequestHandler::HTTP_GetEnvironment(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(match);
    Q_UNUSED(body);
    try{
        res->writeHead(200);
        res->setProperty("Content-Type", "application/json");
        res->end( AgentToUI::formatAgent( Environment::getInstance() , AgentToUI::UPDATE ) );
    } catch(...){
        res->writeHead( QHttpResponse::STATUS_INTERNAL_SERVER_ERROR );
        res->end();
    }
    req->deleteLater();
}

void RequestHandler::HTTP_UpdateEnvironment(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(match);
    try{
        Environment::getInstance()->fromJSON( body );
        res->writeHead(200);
        res->setProperty("Content-Type", "application/json");
        res->end( AgentToUI::formatAgent( Environment::getInstance() , AgentToUI::UPDATE ) );
    } catch(...){
        res->writeHead( QHttpResponse::STATUS_INTERNAL_SERVER_ERROR );
        res->end();
    }
    req->deleteLater();
}

void RequestHandler::HTTP_GetClassInfo(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    Q_UNUSED(match);
    try{

        //Agent* tmp = new Agent();
        res->writeHead(200);
        res->setProperty("Content-Type", "application/json");
        res->end( );
        //tmp->deleteLater();
    } catch(...){
        res->writeHead( QHttpResponse::STATUS_INTERNAL_SERVER_ERROR );
        res->end();
    }
    req->deleteLater();
}

void RequestHandler::HTTP_GetAll(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    try{
        if( Environment::getInstance()->getAgentClasses().contains( match.captured("class_name")) ){
            QList<Agent*> paginated_agents;
            QList<Agent*> agents = Environment::getInstance()->getAgents( match.captured("class_name") );
            for(int i = match.captured("from").toInt() ; i < agents.size() && i < match.captured("to").toInt() ; i++){
                paginated_agents.append( agents.at(i) );
            }
            res->writeHead( QHttpResponse::STATUS_OK );
            res->setProperty("Content-Type", "application/json");
            res->end( AgentToUI::formatAgent( paginated_agents , AgentToUI::UPDATE ) );
        } else {
            res->writeHead( QHttpResponse::STATUS_NOT_FOUND );
            res->end();
        }
    } catch(...){
        res->writeHead( QHttpResponse::STATUS_INTERNAL_SERVER_ERROR );
        res->end();
    }
    req->deleteLater();
}

void RequestHandler::HTTP_GetOne(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    try{
        Agent* agent = Environment::getInstance()->getAgent(match.captured("class_name"), match.captured("id").toInt());
        if(agent){
            res->writeHead( QHttpResponse::STATUS_OK );
            res->setProperty("Content-Type", "application/json");
            res->end( AgentToUI::formatAgent(agent , AgentToUI::UPDATE ) );
        } else {
            res->writeHead( QHttpResponse::STATUS_NOT_FOUND );
            res->end();
        }
    } catch(...){
        res->writeHead( QHttpResponse::STATUS_INTERNAL_SERVER_ERROR );
        res->end();
    }
    req->deleteLater();
}

void RequestHandler::HTTP_Create(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(match);
    Q_UNUSED(body);
    try{
        qDebug() << "Received" << body;
        res->writeHead( QHttpResponse::STATUS_OK );
        res->setProperty("Content-Type", "application/json");
        res->end( );
    } catch(...){
        res->writeHead( QHttpResponse::STATUS_INTERNAL_SERVER_ERROR );
        res->end();
    }
    req->deleteLater();
}

void RequestHandler::HTTP_Update(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    try{
        Agent* agent = Environment::getInstance()->getAgent(match.captured("class_name"), match.captured("id").toInt());
        if(agent){
            agent->fromJSON( body );
            res->writeHead( QHttpResponse::STATUS_OK );
            res->setProperty("Content-Type", "application/json");
            res->end( AgentToUI::formatAgent(agent , AgentToUI::UPDATE ) );
        } else {
            res->writeHead( QHttpResponse::STATUS_NOT_FOUND );
            res->setProperty("Content-Type", "application/json");
            res->end();
        }
    } catch(...){
        res->writeHead( QHttpResponse::STATUS_INTERNAL_SERVER_ERROR );
        res->end();
    }
    req->deleteLater();
}

void RequestHandler::HTTP_RunAll(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    try{
        if( Environment::getInstance()->getAgentClasses().contains( match.captured("class_name")) ){
            QList<Agent*> agents = Environment::getInstance()->getAgents( match.captured("class_name") );
            for(int i = 0; i < agents.size() ; i++){
                QTimer::singleShot( i*100 , agents.at(i) , SLOT( run() ) );
            }
            res->writeHead( QHttpResponse::STATUS_OK );
            res->setProperty("Content-Type", "application/json");
            res->end();
        } else {
            res->writeHead( QHttpResponse::STATUS_NOT_FOUND );
            res->end();
        }
    } catch(...){
        res->writeHead( QHttpResponse::STATUS_INTERNAL_SERVER_ERROR );
        res->end();
    }
    req->deleteLater();
}

void RequestHandler::HTTP_RunOne(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    try{
        Agent* agent = Environment::getInstance()->getAgent(match.captured("class_name"), match.captured("id").toInt());
        if(agent){
            QTimer::singleShot(0 , agent , SLOT( run() ) );
            res->writeHead( QHttpResponse::STATUS_OK );
            res->setProperty("Content-Type", "application/json");
            res->end( AgentToUI::formatAgent( agent, AgentToUI::UPDATE ) );
        } else {
            res->writeHead( QHttpResponse::STATUS_NOT_FOUND );
            res->end();
        }
    } catch(...){
        res->writeHead( QHttpResponse::STATUS_INTERNAL_SERVER_ERROR );
        res->end();
    }
    req->deleteLater();
}

void RequestHandler::HTTP_Stop(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    try{
        Agent* agent = Environment::getInstance()->getAgent(match.captured("class_name"), match.captured("id").toInt());
        if(agent){
            agent->thread()->terminate();
            res->writeHead( QHttpResponse::STATUS_OK );
            res->setProperty("Content-Type", "application/json");
            res->end( AgentToUI::formatAgent(agent , AgentToUI::UPDATE ) );
        } else {
            res->writeHead( QHttpResponse::STATUS_NOT_FOUND );
            res->end();
        }
    } catch(...){
        res->writeHead( QHttpResponse::STATUS_INTERNAL_SERVER_ERROR );
        res->end();
    }
    req->deleteLater();
}

void RequestHandler::HTTP_Delete(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    try{
        Agent* agent = Environment::getInstance()->getAgent(match.captured("class_name"), match.captured("id").toInt());
        if (agent){
            Environment::getInstance()->removeAgent( agent->getId() );
            res->writeHead( QHttpResponse::STATUS_OK );
            res->setProperty("Content-Type", "application/json");
            res->end( AgentToUI::formatAgent( agent , AgentToUI::DELETE ) );
            agent->deleteLater();
        } else {
            res->writeHead( QHttpResponse::STATUS_NOT_FOUND );
            res->end();
        }
    } catch(...){
        res->writeHead( QHttpResponse::STATUS_INTERNAL_SERVER_ERROR );
        res->end();
    }
    req->deleteLater();
}

void RequestHandler::HTTP_Home(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(match);
    Q_UNUSED(body);

    // Read index file
    try{
        QFile file("../web/index.html");
        QByteArray filecontent = "";
        if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)){
            res->writeHead( QHttpResponse::STATUS_NOT_FOUND );
            res->end();
        } else {
            QTextStream in(&file);
            while ( !in.atEnd() ) {
                filecontent += in.readLine();
            }
            res->writeHead( QHttpResponse::STATUS_OK );
            res->setHeader("Content-Length", QString::number(filecontent.size()));
            res->end(filecontent);
            file.close();
        }
    } catch (...){
        res->writeHead( QHttpResponse::STATUS_NOT_FOUND );
        res->end();
    }
    //req->deleteLater();
}

void RequestHandler::HTTP_Static(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body){
    Q_UNUSED(req);
    Q_UNUSED(body);
    /*if (match.captured("file").endsWith("png")){
        QImage* image = new QImage("../web/" + match.captured("file"));
        QByteArray imageByteArray = qCompress(image->bits(), image->byteCount());
        res->writeHead(QHttpResponse::STATUS_OK);
        res->setHeader("Content-Type", "image/png");
        res->end(imageByteArray);
    } else {*/

    // Read the file
    try{
        QFile file("../web/" + match.captured("file"));
        QByteArray filecontent = "";
        if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)){
            res->writeHead( QHttpResponse::STATUS_NOT_FOUND );
            res->end();
        } else {
            QTextStream in(&file);
            while ( !in.atEnd() ) {
                filecontent += in.readLine() + "\n";
            }
            res->writeHead( QHttpResponse::STATUS_OK );
            res->setHeader("Content-Length", QString::number(filecontent.size()));
            res->end( filecontent );
            file.close();
        }
    } catch (...){
        res->writeHead( QHttpResponse::STATUS_NOT_FOUND );
        res->end();
    }
    //req->deleteLater();
}
