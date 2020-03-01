
#include "i_command.h"

namespace dss_client {

using namespace std;

ICommand::ICommand( common_types::SCommandServices * _commandServices )
    : m_commandServices(_commandServices)
{
    m_networkRequest = m_commandServices->network->getRequestInstance();
}

ICommand::~ICommand(){

}

// async section
bool ICommand::execAsync(){

    if( ! serializeRequestTemplateMethodPart() ){
        return false;
    }

    if( ! performAsyncNetworking() ){
        return false;
    }

    return true;
}

bool ICommand::isReady(){

    if( ! m_networkRequest->checkResponseReadyness(m_corrId) ){
        return false;
    }

    m_incomingMsg = m_networkRequest->getAsyncResponse( m_corrId );

    if( ! parseResponseTemplateMethodPart() ){
        return false;
    }

    return true;
}

bool ICommand::performAsyncNetworking(){

    m_corrId = m_networkRequest->sendMessageAsync( m_outcomingMsg );
    return true;
}

// sync section
bool ICommand::exec(){

    if( ! serializeRequestTemplateMethodPart() ){
        return false;
    }

    if( ! performBlockedNetworking() ){
        return false;
    }

    if( ! parseResponseTemplateMethodPart() ){
        return false;
    }

    return true;
}

bool ICommand::performBlockedNetworking(){

    PEnvironmentRequest request = m_commandServices->network->getRequestInstance();
    request->setOutcomingMessage( m_outcomingMsg );
    m_incomingMsg = request->getIncomingMessage();
    return true;
}

}
