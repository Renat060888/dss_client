
#include <microservice_common/system/logger.h>

#include "cmd_ping.h"
#include "cmd_register.h"

namespace dss_client {

using namespace std;

CommandPing::CommandPing( common_types::SCommandServices * _commandServices, PNetworkClient _network )
    : ICommand(_commandServices, _network)
{

}

bool CommandPing::serializeRequestTemplateMethodPart(){

    Json::Value rootRecord;
    rootRecord[ "user_id" ] = ( m_toDSS ? m_userIdToDSS : m_userIdToPlayer );
    rootRecord[ "cmd_type" ] = "service";
    rootRecord[ "cmd_name" ] = "ping";

    m_outcomingMsg = m_jsonWriter.write( rootRecord );

    return true;
}

bool CommandPing::parseResponseTemplateMethodPart(){

    Json::Value parsedRecord;
    if( ! m_jsonReader.parse( m_incomingMsg.c_str(), parsedRecord, false ) ){
        VS_LOG_ERROR << "parse failed due to [" << m_jsonReader.getFormattedErrorMessages() << "]"
                     << " msg [" << m_incomingMsg << "]"
                     << endl;
        return false;
    }

    m_commandServices->clientController->pongCatched();

    if( m_toDSS ){
        if( m_commandServices->clientController->isRegisteredInPlayer() ){

            const void * playerState;

            // parse from message

            m_commandServices->clientController->updatePlayer( playerState );
        }
        else{
            m_commandServices->clientController->registerInPlayer();
        }
    }
    else{
        if( m_commandServices->clientController->isRegisteredInPlayer() ){

            const void * playerState;

            // parse from message

            m_commandServices->clientController->updatePlayer( playerState );
        }
        else{
            m_commandServices->clientController->registerInPlayer();
        }
    }

    return true;
}

}










