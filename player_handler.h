#ifndef PLAYER_HANDLER_H
#define PLAYER_HANDLER_H

#include <memory>

#include "common_types_public.h"

namespace dss_client{

class PlayerHandler
{
public:
    enum class EPlayerStatus {
        INITED,
        PREPARING,
        READY,

        PLAYING,
        ALL_STEPS_PASSED,
        NOT_ENOUGH_STEPS,
        PAUSED,
        STOPPED,
        PLAY_FROM_POSITION,
        CLOSE,

        ACTIVE,
        IDLE,

        CRASHED,
        UNDEFINED
    };

    struct SState {
        SState()
            : status(EPlayerStatus::UNDEFINED)
        {}
        std::string id;
        EPlayerStatus status;
        std::string lastError;

    };

    PlayerHandler();
    ~PlayerHandler();

    const SState & getPlayerState();

    void start();
    void pause();
    void stop();
    bool stepForward();
    bool stepBackward();
    bool setRange( const common_types::TTimeRangeMillisec & _range );
    void switchReverseMode( bool _reverse );
    void switchLoopMode( bool _loop );
    void switchLiveMode( bool _realtime );
    bool playFromPosition( int64_t _stepMillisec );
    bool increasePlayingSpeed();
    bool decreasePlayingSpeed();
    void normalizePlayingSpeed();


private:
    // private stuff
    class PrivateImplementation * m_impl;
};
using PPlayerHandler = std::shared_ptr<PlayerHandler>;

}

#endif // PLAYER_HANDLER_H
