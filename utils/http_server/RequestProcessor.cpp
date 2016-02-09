#include <QDebug>
#include "RequestProcessor.h"

RequestProcessor::RequestProcessor(QHttpRequest::HttpMethod method, QRegularExpression urlRegex, RequestHandler* handler) : QObject()
{
    this->method = method;
    this->url_regex = urlRegex;
    this->handler = handler;
}

RequestProcessor::~RequestProcessor(){
}


QRegularExpressionMatch RequestProcessor::match(QUrl url)
{
    QRegularExpressionMatch match = url_regex.match( url.path() );
    return match;
}

void RequestProcessor::storeReferencesUntillReqEnd(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch matched_regex){
    RequestRef ref;
    ref.request = req;
    ref.response = res;
    ref.matched_regex = matched_regex;
    this->request_references.append(ref);
}

/**********************************************************************
 GETTERS
**********************************************************************/

QHttpRequest::HttpMethod RequestProcessor::getMethod() const{
    return this->method;
}

 QRegularExpression RequestProcessor::getUrlRegex() const{
    return this->url_regex;
}

/**********************************************************************
 SLOTS
**********************************************************************/

void RequestProcessor::requestEnded(){
    // Now parse body
    QJsonObject body;
    try {

        if( this->request_references.isEmpty() ){
            return;
        }

        // Else get the first reference
        RequestRef ref = this->request_references.takeAt( 0 );
        QByteArray b = ref.request->body();
        if( !b.isEmpty() ){
            body = QJsonDocument::fromJson( b ).object();
        }

        // Call the SLOT from HttpHandler that will be waiting these elements
        emit matched(ref.request, ref.response, ref.matched_regex, body);

    } catch(...){qWarning() << "[RequestProcessor::requestEnded] Error parsing body"; }

}
