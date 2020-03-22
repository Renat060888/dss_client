
#include <microservice_common/system/logger.h>

#include "cmd_context_open.h"

namespace dss_client {

using namespace std;

CommandContextOpen::CommandContextOpen( common_types::SCommandServices * _commandServices, PNetworkClient _network )
    : ICommand(_commandServices, _network)
{

}

bool CommandContextOpen::serializeRequestTemplateMethodPart(){

    Json::Value rootRecord;
    rootRecord[ "client_id" ] = m_clientId;
    rootRecord[ "cmd_type" ] = "service";
    rootRecord[ "cmd_name" ] = "context_open";
    rootRecord[ "context_name" ] = m_contextName;

    m_outcomingMsg = m_jsonWriter.write( rootRecord );

    return true;
}

bool CommandContextOpen::parseResponseTemplateMethodPart(){

    Json::Value parsedRecord;
    if( ! m_jsonReader.parse( m_incomingMsg.c_str(), parsedRecord, false ) ){
        VS_LOG_ERROR << "parse failed due to [" << m_jsonReader.getFormattedErrorMessages() << "]"
                     << " msg [" << m_incomingMsg << "]"
                     << endl;
        return false;
    }




    return true;
}

}
