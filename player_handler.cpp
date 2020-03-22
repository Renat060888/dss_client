
#include "player_handler.h"
#include "commands/player/cmd_play_from_pos.h"
#include "commands/player/cmd_play_start.h"
#include "commands/player/cmd_play_pause.h"
#include "commands/player/cmd_play_stop.h"
#include "commands/player/cmd_play_loop.h"
#include "commands/player/cmd_play_reverse.h"
#include "commands/player/cmd_play_speed.h"
#include "commands/player/cmd_play_step.h"
#include "commands/player/cmd_play_realtime.h"

using namespace std;

namespace dss_client{

using namespace common_types;

// --------------------------------------------------------------------
// private
// --------------------------------------------------------------------
class PrivateImplementation {
public:
    PrivateImplementation()
    {}

    PlayerHandler::SState state;
    SCommandServices commandServices;
};

// --------------------------------------------------------------------
// public
// --------------------------------------------------------------------
PlayerHandler::PlayerHandler()
{

}

PlayerHandler::~PlayerHandler()
{

}

const PlayerHandler::SState & PlayerHandler::getPlayerState(){
    return m_impl->state;
}

void PlayerHandler::start(){

    PCommandPlayStart cmd = std::make_shared<CommandPlayStart>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );

    cmd->exec();
}

void PlayerHandler::pause(){

    PCommandPlayPause cmd = std::make_shared<CommandPlayPause>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );

    cmd->exec();
}

void PlayerHandler::stop(){

    PCommandPlayStop cmd = std::make_shared<CommandPlayStop>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );

    cmd->exec();
}

bool PlayerHandler::stepForward(){

    PCommandPlayStep cmd = std::make_shared<CommandPlayStep>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    cmd->stepForward = true;

    return cmd->exec();
}

bool PlayerHandler::stepBackward(){

    PCommandPlayStep cmd = std::make_shared<CommandPlayStep>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    cmd->stepForward = false;

    return cmd->exec();
}

bool PlayerHandler::setRange( const TTimeRangeMillisec & _range ){
    // TODO: do
}

void PlayerHandler::switchReverseMode( bool _reverse ){

    PCommandPlayReverse cmd = std::make_shared<CommandPlayReverse>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    cmd->reverse = _reverse;

    cmd->exec();
}

void PlayerHandler::switchLoopMode( bool _loop ){

    PCommandPlayLoop cmd = std::make_shared<CommandPlayLoop>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    cmd->loop = _loop;

    cmd->exec();
}

void PlayerHandler::switchLiveMode( bool _realtime ){

    PCommandPlayLive cmd = std::make_shared<CommandPlayLive>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    cmd->live = _realtime;

    cmd->exec();
}

bool PlayerHandler::playFromPosition( int64_t _stepMillisec ){

    PCommandPlayFromPos cmd = std::make_shared<CommandPlayFromPos>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    cmd->targetPosMillisec = _stepMillisec;

    return cmd->exec();
}

bool PlayerHandler::increasePlayingSpeed(){

    PCommandPlaySpeed cmd = std::make_shared<CommandPlaySpeed>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    cmd->increase = true;

    return cmd->exec();
}

bool PlayerHandler::decreasePlayingSpeed(){

    PCommandPlaySpeed cmd = std::make_shared<CommandPlaySpeed>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    cmd->increase = false;

    return cmd->exec();
}

void PlayerHandler::normalizePlayingSpeed(){

    PCommandPlaySpeed cmd = std::make_shared<CommandPlaySpeed>( & m_impl->commandServices, m_impl->commandServices.networkClientForPlayer );
    cmd->normalize = true;

    cmd->exec();
}

}




