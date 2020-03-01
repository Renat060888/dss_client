#ifndef HANDLER_PLAYER_H
#define HANDLER_PLAYER_H

#include <memory>

namespace dss_client {

class HandlerPlayer
{
public:
    struct SState {

    };

    HandlerPlayer();

    const SState & getState();

    bool start();
    bool pause();
    bool stop();

    bool stepForward();
    bool stepBackward();
    bool fromStep( int _step );

    bool speedIncrease();
    bool speedDecrease();
    bool speedNormalize();

    bool setRange();
    bool switchLoopMode( bool _loop );
    bool switchReverseMode( bool _reverse );

private:
    class SPrivateImplementation * m_impl;

};
using PHandlerPlayer = std::shared_ptr<HandlerPlayer>;

}

#endif // HANDLER_PLAYER_H
