
#include <microservice_common/system/logger.h>

#include "cmd_ping.h"

namespace dss_client {

using namespace std;

CommandPing::CommandPing( common_types::SCommandServices * _commandServices )
    : ICommand(_commandServices)
{

}

bool CommandPing::serializeRequestTemplateMethodPart(){

    Json::Value rootRecord;
    rootRecord[ "user_id" ] = m_userId;
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



    VS_LOG_INFO << "parsed msg [" << m_incomingMsg << "]" << endl;

    m_commandServices;



    return true;
}

}










