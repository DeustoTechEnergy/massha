#ifndef REQUESTPROCESSOR_H
#define REQUESTPROCESSOR_H

#include <QMap>
#include <QString>
#include <QRegularExpression>
#include <QUrl>
#include <QJsonObject>
#include <QJsonDocument>
#include <qhttpresponse.h>

#include "utils/http_server/RequestHandler.h"

QT_FORWARD_DECLARE_CLASS(RequestHandler)

struct RequestRef{
    QHttpRequest* request;
    QHttpResponse* response;
    QRegularExpressionMatch matched_regex;
};

/**
 * @brief The RequestProcessor class processes the request that matches a given http_method and urlRegex.
 * When the processor starts, the RequestHandler gets locked untill the processor unlocks it.
 * This is because if the request has a body, the requestprocessor needs to completely read it, and then unlock
 * the handler.
 */
class RequestProcessor : public QObject
{
     Q_OBJECT
public:
    RequestProcessor(QHttpRequest::HttpMethod method, QRegularExpression url_regex, RequestHandler* http_handler);
    ~RequestProcessor();
    QRegularExpressionMatch match(QUrl url);
    void storeReferencesUntillReqEnd(QHttpRequest *request, QHttpResponse *response, QRegularExpressionMatch matched_regex);

    //Getters
    QHttpRequest::HttpMethod getMethod() const;
    QRegularExpression getUrlRegex() const;

signals:
    void matched(QHttpRequest *request, QHttpResponse *response, QRegularExpressionMatch matched_regex, QJsonObject body);

public slots:
    void requestEnded();

private:
    QHttpRequest::HttpMethod method; // HTTP METHOD (GET, POST, PUT, DELETE)
    QRegularExpression url_regex; // URL REGEX

    // The RequestHandler that created this urlmatch, to notify it when the UrlMatch has finished
    RequestHandler* handler;

    // Store request references, untill request has finished
    QList<RequestRef> request_references;

};

#endif // REQUESTPROCESSOR_H
