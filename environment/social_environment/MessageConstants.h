#ifndef MESSAGECONSTANTS
#define MESSAGECONSTANTS

#include <QObject>

class MessageConstants : public QObject
{
    Q_OBJECT
    Q_ENUMS(message_types)
public:

    enum message_types {
        REQUEST,
        INFORM,
        QUERY,
        PROPOSE
    };
};

#endif // MESSAGECONSTANTS

