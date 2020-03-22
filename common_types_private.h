#ifndef COMMON_TYPES_PRIVATE_H
#define COMMON_TYPES_PRIVATE_H

#include <microservice_common/communication/network_interface.h>

#include "common_types_public.h"

namespace dss_client {
namespace common_types {


// typedefs
using TDssClientUniqueId = std::string;
using TPlayerClientUniqueId = std::string;


// interfaces
class IClientControl {
public:
    virtual ~IClientControl(){}

    virtual void pongCatched() = 0;

    virtual bool isRegisteredInDSS() = 0;
    virtual void registerInDSS() = 0;
    virtual void updateNodes( const std::vector<void *> & _nodeStates ) = 0;

    virtual bool isRegisteredInPlayer() = 0;
    virtual void registerInPlayer() = 0;
    virtual void setIdFromPlayer( const TDssClientUniqueId & _id ) = 0;
    virtual void updatePlayer( const void * & _playerState ) = 0;
};


// containers
struct SCommandServices {
    PNetworkClient networkClientForDss;
    PNetworkClient networkClientForPlayer;
    IClientControl * clientController;
};



}
}

#endif // COMMON_TYPES_PRIVATE_H
