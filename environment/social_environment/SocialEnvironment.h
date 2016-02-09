#ifndef SOCIALENVIRONMENT_H
#define SOCIALENVIRONMENT_H

#include "agents/Agent.h"

class SocialEnvironment
{
public:

protected:
    explicit SocialEnvironment();
    ~SocialEnvironment();

    // ENVIRONMENT METHODS
    bool addAgent( Agent* agent );

};

#endif // SOCIALENVIRONMENT_H
