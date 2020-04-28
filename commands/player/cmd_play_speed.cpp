
#include <iostream>

#include <jsoncpp/json/writer.h>
#include <jsoncpp/json/reader.h>

#include "common_vars.h"
#include "cmd_play_speed.h"

using namespace std;

namespace dss_client{

using namespace common_types;

CommandPlaySpeed::CommandPlaySpeed( SCommandServices * _commandServices, PNetworkClient _network )
    : ICommand(_commandServices, _network)
{

}

CommandPlaySpeed::~CommandPlaySpeed()
{

}

bool CommandPlaySpeed::serializeRequestTemplateMethodPart(){

    Json::Value root;
    root[ "cmd_type" ] = "analyze";
    root[ "cmd_name" ] = "play_with_speed";
    root[ "increase" ] = increase;
    root[ "normalize" ] = normalize;
    root[ "user_id" ] = m_userIdToPlayer;

    Json::FastWriter writer;
    m_outcomingMsg = writer.write( root );

    return true;
}

bool CommandPlaySpeed::parseResponseTemplateMethodPart(){

    Json::Value parsedRecord;
    if( ! m_jsonReader.parse( m_incomingMsg.c_str(), parsedRecord, false ) ){

        cerr << PRINT_HEADER
             << "Command: parse failed of [1] Reason: [2] "
             << m_incomingMsg << " " << m_jsonReader.getFormattedErrorMessages()
             << endl;

        m_lastError = "";
        return false;
    }


    return true;
}

}
