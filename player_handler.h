#ifndef PLAYER_HANDLER_H
#define PLAYER_HANDLER_H

#include <memory>

#include <boost/signals2.hpp>

#include "common_types_public.h"
#include "common_types_private.h"

namespace dss_client{

class PlayerHandler
{
    friend class PrivateImplementationDC;
public:   
    struct SInitSettings {
        common_types::TPlayerClientUniqueId userId;
    };

    struct SState {
        SState()        
        {}

        common_types::SPlayingServiceState playingState;
        SInitSettings settings;
        std::string lastError;
    };
    using PState = std::shared_ptr<SState>;
    using PConstState = std::shared_ptr<const SState>;

    boost::signals2::signal<void()> m_stateUpdated;

    PlayerHandler( common_types::SCommandServices & _commandServices );
    ~PlayerHandler();

    bool init( const SInitSettings & _settings );
    const SState & getStateRef();
    const PState getStatePtr();
    const PConstState getStateConstPtr();

    void start();
    void pause();
    void stop();
    bool stepForward();
    bool stepBackward();

    bool playFromPosition( int64_t _stepMillisec );
    bool increasePlayingSpeed();
    bool decreasePlayingSpeed();
    void normalizePlayingSpeed();
    bool setRange( const common_types::TTimeRangeMillisec & _range );

    void switchReverseMode( bool _reverse );
    void switchLoopMode( bool _loop );
    void switchLiveMode( bool _realtime );


private:
    // private stuff
    class PrivateImplementationPH * m_impl;

    void updateState( const common_types::SPlayingServiceState & _state );
};
using PPlayerHandler = std::shared_ptr<PlayerHandler>;

}

#endif // PLAYER_HANDLER_H
