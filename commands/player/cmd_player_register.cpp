
#include "from_ms_common/system/logger.h"

#include "cmd_player_register.h"

using namespace std;

namespace dss_client {

CommandPlayerRegister::CommandPlayerRegister( common_types::SCommandServices * _commandServices, PNetworkClient _network )
    : ICommand(_commandServices, _network)
{

}

bool CommandPlayerRegister::serializeRequestTemplateMethodPart(){

    Json::Value rootRecord;
    rootRecord[ "cmd_type" ] = "service";
    rootRecord[ "cmd_name" ] = "register";
    rootRecord[ "user_ip" ] = m_userIpStr;
    rootRecord[ "user_pid" ] = m_userPid;

    m_outcomingMsg = m_jsonWriter.write( rootRecord );

    return true;
}

bool CommandPlayerRegister::parseResponseTemplateMethodPart(){

    Json::Value parsedRecord;
    if( ! m_jsonReader.parse( m_incomingMsg.c_str(), parsedRecord, false ) ){
        VS_LOG_ERROR << "parse failed due to [" << m_jsonReader.getFormattedErrorMessages() << "]"
                     << " msg [" << m_incomingMsg << "]"
                     << endl;
        return false;
    }

    const common_types::TDssClientUniqueId userId = parsedRecord["user_id"].asString();
    m_commandServices->clientController->setIdFromPlayer( userId );

    return true;
}

}
