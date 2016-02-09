#ifndef REQUESTPROCESSORCREATOR_H
#define REQUESTPROCESSORCREATOR_H

#include <QStringList>

#include "utils/http_server/RequestHandler.h"

/**
 * @brief The RequestProcessorCreator creates a RequestProcessor for every available url
 * and connects them to the handler.
 */
class RequestProcessorCreator : public QObject
{
    Q_OBJECT
public:

    RequestProcessorCreator(RequestHandler* handler);
    void createUrlsForAgents(RequestHandler* handler, QString class_name);
};

#endif // REQUESTPROCESSORCREATOR_H
