#ifndef SKILLCONSTANTS
#define SKILLCONSTANTS

#include <QObject>

class SkillConstants : public QObject
{
    Q_OBJECT
    Q_ENUMS(capabilities)
public:

    enum capabilities {
        MOVE,           // Change its position
        COMMUNICATE     //
    };
};

#endif // SKILLCONSTANTS

