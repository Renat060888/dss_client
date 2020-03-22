#ifndef HANDLER_NODE_REAL_H
#define HANDLER_NODE_REAL_H

#include <memory>

namespace dss_client {

class HandlerNodeReal
{
public:
    struct SState {

    };

    HandlerNodeReal();

    const SState & getState();

    bool configure();

    bool start();
    bool pause();
    bool reset();


private:
    // private stuff
    class SPrivateImplementation * m_impl;

};
using PHandlerNodeReal = std::shared_ptr<HandlerNodeReal>;

}

#endif // HANDLER_NODE_REAL_H
