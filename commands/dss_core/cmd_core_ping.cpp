
#include "from_ms_common/system/logger.h"

#include "common_vars.h"
#include "cmd_core_register.h"
#include "cmd_core_ping.h"

namespace dss_client {

using namespace std;

CommandDSSPing::CommandDSSPing( common_types::SCommandServices * _commandServices, PNetworkClient _network )
    : ICommand(_commandServices, _network)
{

}

bool CommandDSSPing::serializeRequestTemplateMethodPart(){

    Json::Value rootRecord;
    rootRecord[ "user_id" ] = m_userIdToDSS;
    rootRecord[ "cmd_type" ] = "service";
    rootRecord[ "cmd_name" ] = "ping";

    m_outcomingMsg = m_jsonWriter.write( rootRecord );

    return true;
}

bool CommandDSSPing::parseResponseTemplateMethodPart(){

    Json::Value parsedRecord;
    if( ! m_jsonReader.parse( m_incomingMsg.c_str(), parsedRecord, false ) ){
        VS_LOG_ERROR << "parse failed due to [" << m_jsonReader.getFormattedErrorMessages() << "]"
                     << " msg [" << m_incomingMsg << "]"
                     << endl;
        return false;
    }

    m_commandServices->clientController->pongByDSSCatched();

    const string code = parsedRecord["code"].asString();

    if( code != common_vars::DSS_CODE_NOT_REGISTERED ){

        vector<void *> nodesState;

        // parse from message

        m_commandServices->clientController->updateNodes( nodesState );
    }
    else{
        m_commandServices->clientController->registerInDSS();
    }

    return true;
}

}










