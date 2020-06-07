
#include "from_ms_common/system/logger.h"

#include "common_utils.h"
#include "common_vars.h"
#include "cmd_player_register.h"
#include "cmd_player_ping.h"

namespace dss_client {

using namespace std;
using namespace common_types;

static bool readState( const Json::Value & _record, SPlayingServiceState & _newState ){

    _newState.info.stepIntervalMillisec = _record["step_interval"].asInt64();
    _newState.info.currentStepMillisec = _record["current_step"].asInt64();
    _newState.info.globalRangeMillisec.first = _record["global_range_left"].asInt64();
    _newState.info.globalRangeMillisec.second = _record["global_range_right"].asInt64();

    const Json::Value & dataSets = _record["datasets"];
    for( int i = 0; i < dataSets.size(); i++ ){
        const Json::Value & arrElement = dataSets[ i ];

        common_types::SPlayingDataSet dataSet;
        dataSet.uniqueId = arrElement["unique_id"].asInt64();
        dataSet.real = arrElement["real"].asBool();
//        dataSet.description = arrElement["descr"].asString();

        const Json::Value & ranges = arrElement["ranges"];
        for( int i = 0; i < ranges.size(); i++ ){
            const Json::Value & arrElement = ranges[ i ];

            dataSet.dataRanges.push_back( {arrElement["range_left"].asInt64(), arrElement["range_right"].asInt64()} );
        }

        _newState.info.playingData.push_back( dataSet );
    }

    return true;
}

CommandPlayerPing::CommandPlayerPing( common_types::SCommandServices * _commandServices, PNetworkClient _network )
    : ICommand(_commandServices, _network)
{

}

bool CommandPlayerPing::serializeRequestTemplateMethodPart(){

    Json::Value rootRecord;
    rootRecord[ "user_id" ] = m_userIdToPlayer;
    rootRecord[ "cmd_type" ] = "service";
    rootRecord[ "cmd_name" ] = "ping";

    m_outcomingMsg = m_jsonWriter.write( rootRecord );

    return true;
}

bool CommandPlayerPing::parseResponseTemplateMethodPart(){

    Json::Value parsedRecord;
    if( ! m_jsonReader.parse( m_incomingMsg.c_str(), parsedRecord, false ) ){
        VS_LOG_ERROR << "parse failed due to [" << m_jsonReader.getFormattedErrorMessages() << "]"
                     << " msg [" << m_incomingMsg << "]"
                     << endl;
        return false;
    }

//    cout << "pong from player: " << m_incomingMsg << endl;

    // pong detect
    m_commandServices->clientController->pongByPlayerCatched();

    // read state
    const string code = parsedRecord["code"].asString();
    if( code != common_vars::PLAYER_CODE_NOT_REGISTERED ){
        SPlayingServiceState newState;
        bool stateHasCome = readState( parsedRecord["player_state"], newState );
        m_commandServices->clientController->updatePlayer( stateHasCome, newState );
    }
    else{
        m_commandServices->clientController->registerInPlayer();
    }

    return true;
}

}










