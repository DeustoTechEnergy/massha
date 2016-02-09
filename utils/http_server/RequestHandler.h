#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QUrl>
#include <QVector>
#include <QMutex>
#include <qhttprequest.h>
#include <qhttpresponse.h>
#include <QJsonObject>

#include "utils/http_server/RequestProcessor.h"

QT_FORWARD_DECLARE_CLASS(RequestProcessor)

class RequestHandler : public QObject
{
     Q_OBJECT

public:
    RequestHandler();
    RequestProcessor* addUrlMatch(QHttpRequest::HttpMethod method, QRegularExpression urlRegex);
    bool containsUrlMath(QHttpRequest::HttpMethod method, QRegularExpression urlRegex);

public slots:
    // MAIN HANDLER
    void handleHttpRequest(QHttpRequest *req, QHttpResponse *resp);

    // AGENT SLOTS
    void HTTP_GetClassInfo(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body);
    void HTTP_GetAll(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body);
    void HTTP_GetOne(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body);
    void HTTP_Create(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body);
    void HTTP_Update(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body);
    void HTTP_RunAll(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body);
    void HTTP_RunOne(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body);
    void HTTP_Stop(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body);
    void HTTP_Delete(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body);

    // SERVER SLOTS
    void HTTP_Home(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body);
    void HTTP_Static(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body);

    // ENVIRONMENT SLOTS
    void HTTP_GetEnvironment(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body);
    void HTTP_UpdateEnvironment(QHttpRequest *req, QHttpResponse *res, QRegularExpressionMatch match, QJsonObject body);

private:
    QMap<QHttpRequest::HttpMethod, QVector<RequestProcessor*> >urls;
};

#endif // REQUESTHANDLER_H
