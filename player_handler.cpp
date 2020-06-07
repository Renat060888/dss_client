
#include "common_utils.h"
#include "from_ms_common/system/logger.h"

#include "player_handler.h"
#include "commands/player/cmd_play_from_pos.h"
#include "commands/player/cmd_play_start.h"
#include "commands/player/cmd_play_pause.h"
#include "commands/player/cmd_play_stop.h"
#include "commands/player/cmd_play_loop.h"
#include "commands/player/cmd_play_reverse.h"
#include "commands/player/cmd_play_speed.h"
#include "commands/player/cmd_play_step.h"
#include "commands/player/cmd_play_live.h"

namespace dss_client{

using namespace std;
using namespace common_types;

// --------------------------------------------------------------------
// private
// --------------------------------------------------------------------
class PrivateImplementationPH {
public:
    PrivateImplementationPH( SCommandServices & _commandServices )
        : commandServices(_commandServices)
    {}



    // data
    PlayerHandler::PState state;

    // service
    PlayerHandler * interface;
    SCommandServices & commandServices;





};

// --------------------------------------------------------------------
// public
// --------------------------------------------------------------------
PlayerHandler::PlayerHandler( SCommandServices & _commandServices )
    : m_impl(new PrivateImplementationPH(_commandServices))
{
    m_impl->interface = this;
    m_impl->state = std::make_shared<SState>();
}

PlayerHandler::~PlayerHandler()
{
    delete m_impl;
    m_impl = nullptr;
}

void PlayerHandler::updateState( const common_types::SPlayingServiceState & _state ){

    m_impl->state->playingState = _state;

    // step number to step time
    m_impl->state->playingState.info.currentStepMillisec = m_impl->state->playingState.info.globalRangeMillisec.first
            + ( m_impl->state->playingState.info.currentStepMillisec * m_impl->state->playingState.info.stepIntervalMillisec );

//    VS_LOG_INFO << "curr: " << ::common_utils::timeMillisecToStr(m_impl->state->playingState.info.currentStepMillisec)
//                << endl;

//    for( const SPlayingDataSet & dataSet : m_impl->state->playingState.info.playingData ){
//        for( const TTimeRangeMillisec & range : dataSet.dataRanges ){
//            VS_LOG_INFO << ::common_utils::timeMillisecToStr(range.first) << " " << ::common_utils::timeMillisecToStr(range.second) << " - ";
//        }
//        VS_LOG_INFO << endl;
//        VS_LOG_INFO << "glob: " << ::common_utils::timeMillisecToStr(m_impl->state->playingState.info.globalRangeMillisec.first)
//                    << " " << ::common_utils::timeMillisecToStr(m_impl->state->playingState.info.globalRangeMillisec.second)
//                    << endl;
//    }

    m_stateUpdated();
}

bool PlayerHandler::init( const SInitSettings & _settings ){

    m_impl->state->settings = _settings;


    return true;
}

const PlayerHandler::SState & PlayerHandler::getStateRef(){
    return ( * m_impl->state );
}

const PlayerHandler::PState PlayerHandler::getStatePtr(){
    return m_impl->state;
}

const PlayerHandler::PConstState PlayerHandler::getStateConstPtr(){
    return m_impl->state;
}

void PlayerHandler::start(){

    PCommandPlayStart cmd = std::make_shared<CommandPlayStart>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    cmd->m_userIdToPlayer = m_impl->state->settings.userId;
    cmd->execAsync();

    m_impl->commandServices.clientController->addCommmand( cmd );
}

void PlayerHandler::pause(){

    PCommandPlayPause cmd = std::make_shared<CommandPlayPause>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    cmd->m_userIdToPlayer = m_impl->state->settings.userId;
    cmd->execAsync();

    m_impl->commandServices.clientController->addCommmand( cmd );
}

void PlayerHandler::stop(){

    PCommandPlayStop cmd = std::make_shared<CommandPlayStop>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    cmd->m_userIdToPlayer = m_impl->state->settings.userId;
    cmd->execAsync();

    m_impl->commandServices.clientController->addCommmand( cmd );
}

bool PlayerHandler::stepForward(){

    PCommandPlayStep cmd = std::make_shared<CommandPlayStep>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    cmd->stepForward = true;
    cmd->m_userIdToPlayer = m_impl->state->settings.userId;
    cmd->execAsync();

    m_impl->commandServices.clientController->addCommmand( cmd );
}

bool PlayerHandler::stepBackward(){

    PCommandPlayStep cmd = std::make_shared<CommandPlayStep>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    cmd->stepForward = false;
    cmd->m_userIdToPlayer = m_impl->state->settings.userId;
    cmd->execAsync();

    m_impl->commandServices.clientController->addCommmand( cmd );
}

bool PlayerHandler::setRange( const TTimeRangeMillisec & _range ){
    // TODO: do
}

void PlayerHandler::switchReverseMode( bool _reverse ){

    PCommandPlayReverse cmd = std::make_shared<CommandPlayReverse>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    cmd->reverse = _reverse;
    cmd->m_userIdToPlayer = m_impl->state->settings.userId;
    cmd->execAsync();

    m_impl->commandServices.clientController->addCommmand( cmd );
}

void PlayerHandler::switchLoopMode( bool _loop ){

    PCommandPlayLoop cmd = std::make_shared<CommandPlayLoop>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    cmd->loop = _loop;
    cmd->m_userIdToPlayer = m_impl->state->settings.userId;
    cmd->execAsync();

    m_impl->commandServices.clientController->addCommmand( cmd );
}

void PlayerHandler::switchLiveMode( bool _realtime ){

    PCommandPlayLive cmd = std::make_shared<CommandPlayLive>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    cmd->live = _realtime;
    cmd->m_userIdToPlayer = m_impl->state->settings.userId;
    cmd->execAsync();

    m_impl->commandServices.clientController->addCommmand( cmd );
}

bool PlayerHandler::playFromPosition( int64_t _stepMillisec ){

    PCommandPlayFromPos cmd = std::make_shared<CommandPlayFromPos>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    cmd->targetPosMillisec = _stepMillisec;
    cmd->m_userIdToPlayer = m_impl->state->settings.userId;
    const bool rt = cmd->execAsync();

    m_impl->commandServices.clientController->addCommmand( cmd );
    return rt;
}

bool PlayerHandler::increasePlayingSpeed(){

    PCommandPlaySpeed cmd = std::make_shared<CommandPlaySpeed>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    cmd->increase = true;
    cmd->normalize = false;
    cmd->m_userIdToPlayer = m_impl->state->settings.userId;
    const bool rt = cmd->execAsync();

    m_impl->commandServices.clientController->addCommmand( cmd );
    return rt;
}

bool PlayerHandler::decreasePlayingSpeed(){

    PCommandPlaySpeed cmd = std::make_shared<CommandPlaySpeed>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    cmd->increase = false;
    cmd->normalize = false;
    cmd->m_userIdToPlayer = m_impl->state->settings.userId;
    const bool rt = cmd->execAsync();

    m_impl->commandServices.clientController->addCommmand( cmd );
    return rt;
}

void PlayerHandler::normalizePlayingSpeed(){

    PCommandPlaySpeed cmd = std::make_shared<CommandPlaySpeed>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    cmd->increase = false;
    cmd->normalize = true;
    cmd->m_userIdToPlayer = m_impl->state->settings.userId;
    cmd->execAsync();

    m_impl->commandServices.clientController->addCommmand( cmd );
}

}




