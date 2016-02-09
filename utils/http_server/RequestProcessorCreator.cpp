#include "utils/http_server/RequestProcessor.h"
#include "RequestProcessorCreator.h"

RequestProcessorCreator::RequestProcessorCreator(RequestHandler* handler)
{
    // SERVER URL
    RequestProcessor *processor = handler->addUrlMatch(QHttpRequest::HTTP_GET, QRegularExpression("^/$"));
    QObject::connect(processor, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)), handler, SLOT(HTTP_Home(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)));

    // STATIC CONTENT URL
    processor = handler->addUrlMatch(QHttpRequest::HTTP_GET, QRegularExpression("^/static/(?<file>[a-z0-9/.-]+$)"));
    QObject::connect(processor, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)), handler, SLOT(HTTP_Static(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)));

    // GET ENVIRONMENT URL
    processor = handler->addUrlMatch(QHttpRequest::HTTP_GET, QRegularExpression("^/environment$"));
    QObject::connect(processor, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)), handler, SLOT(HTTP_GetEnvironment(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)));

    // UPDATE ENVIRONMENT URL
    processor = handler->addUrlMatch(QHttpRequest::HTTP_PUT, QRegularExpression("^/environment$"));
    QObject::connect(processor, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)), handler, SLOT(HTTP_UpdateEnvironment(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)));
}

void RequestProcessorCreator::createUrlsForAgents(RequestHandler* handler, QString class_name){
    RequestProcessor* processor;

    // GET CLASS INFO URL
    processor = handler->addUrlMatch(QHttpRequest::HTTP_GET, QRegularExpression("^/agents/(?<class_name>" + class_name + ")/description$"));
    if( processor ){
        QObject::connect(processor, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)), handler, SLOT(HTTP_GetClassInfo(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)));
    }

    // GET ALL AGENTS URL
    processor = handler->addUrlMatch(QHttpRequest::HTTP_GET, QRegularExpression("^/agents/(?<class_name>" + class_name + ")/from/(?<from>[0-9]+)/to/(?<to>[0-9]+$)"));
    if( processor ){
        QObject::connect(processor, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)), handler, SLOT(HTTP_GetAll(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)));
    }

    // GET ONE AGENT URL
    processor = handler->addUrlMatch(QHttpRequest::HTTP_GET, QRegularExpression("^/agents/(?<class_name>" + class_name + ")/(?<id>[a-z0-9]+$)"));
    if( processor ){
        QObject::connect(processor, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)), handler, SLOT(HTTP_GetOne(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)));
    }

    // CREATE ONE AGENT URL
    processor = handler->addUrlMatch(QHttpRequest::HTTP_POST, QRegularExpression("^/agents/(?<class_name>" + class_name + "$)"));
    if( processor ){
        QObject::connect(processor, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)), handler, SLOT(HTTP_Create(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)));
    }

    // UPDATE ONE AGENT URL
    processor = handler->addUrlMatch(QHttpRequest::HTTP_PUT, QRegularExpression("^/agents/(?<class_name>" + class_name + ")/(?<id>[a-z0-9]+$)"));
    if( processor ){
        QObject::connect(processor, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)), handler, SLOT(HTTP_Update(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)));
    }

    // RUN ALL AGENTS URL
    processor = handler->addUrlMatch(QHttpRequest::HTTP_POST, QRegularExpression("^/agents/(?<class_name>" + class_name + ")/run$"));
    if( processor ){
        QObject::connect(processor, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)), handler, SLOT(HTTP_RunAll(QHttpRequest*,QHttpResponse*,QRegularExpressionMatch, QJsonObject)));
    }

    // RUN ONE AGENT URL
    processor = handler->addUrlMatch(QHttpRequest::HTTP_POST, QRegularExpression("^/agents/(?<class_name>" + class_name + ")/(?<id>[a-z0-9]+)/run$"));
    if( processor ){
        QObject::connect(processor, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)), handler, SLOT(HTTP_RunOne(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)));
    }

    // STOP ONE AGENT URL
    processor = handler->addUrlMatch(QHttpRequest::HTTP_POST, QRegularExpression("^/agents/(?<class_name>" + class_name + ")/(?<id>[a-z0-9]+)/stop$"));
    if( processor ){
        QObject::connect(processor, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)), handler, SLOT(HTTP_Stop(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)));
    }

    // DELETE ONE AGENT URL
    processor = handler->addUrlMatch(QHttpRequest::HTTP_DELETE, QRegularExpression("^/agents/(?<class_name>" + class_name + ")/(?<id>[a-z0-9]+$)"));
    if( processor ){
        QObject::connect(processor, SIGNAL(matched(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)), handler, SLOT(HTTP_Delete(QHttpRequest*, QHttpResponse*, QRegularExpressionMatch, QJsonObject)));
    }
}


