#include <QDebug>

#include "SocialEnvironment.h"

SocialEnvironment::SocialEnvironment(){

}

SocialEnvironment::~SocialEnvironment(){
}

/**********************************************************************
 METHODS
**********************************************************************/

bool SocialEnvironment::addAgent(Agent *agent){
    Q_UNUSED(agent);
    // Connect with social environment to send-receive messages
    //SocialEnvironment::connect( agent , SIGNAL( sendMessage(Message*) ) , this, SLOT( transportMessage(Message*) ) );
    return true;
}
