
#include "handler_node_simula.h"

namespace dss_client {

// --------------------------------------------------------------------
// private
// --------------------------------------------------------------------
class PrivateImplementation {
public:
    PrivateImplementation()
    {}

    HandlerNodeSimula::SState state;
};

// --------------------------------------------------------------------
// public
// --------------------------------------------------------------------
HandlerNodeSimula::HandlerNodeSimula()
    : m_impl(new PrivateImplementation())
{

}

HandlerNodeSimula::~HandlerNodeSimula(){

    delete m_impl;
    m_impl = nullptr;
}

const HandlerNodeSimula::SState & HandlerNodeSimula::getState(){
    return m_impl->state;
}

bool HandlerNodeSimula::configure( SConfigSettings _config ){

    // check online



    return true;
}

bool HandlerNodeSimula::start(){

    // check online


    return true;
}

bool HandlerNodeSimula::pause(){

    // check online



    return true;
}

bool HandlerNodeSimula::reset(){

    // check online



    return true;
}

}
