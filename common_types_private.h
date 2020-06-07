#ifndef COMMON_TYPES_PRIVATE_H
#define COMMON_TYPES_PRIVATE_H

#include "from_ms_common/communication/network_interface.h"

#include "common_types_public.h"

namespace dss_client {

class ICommand;
using PCommand = std::shared_ptr<ICommand>;

namespace common_types {


// typedefs
using TDssClientUniqueId = std::string;
using TPlayerClientUniqueId = std::string;


// interfaces
class IClientControl {
public:
    virtual ~IClientControl(){}

    virtual void pongByDSSCatched() = 0;
    virtual void registerInDSS() = 0;
    virtual void setIdFromDSS( const common_types::TDssClientUniqueId & _id ) = 0;
    virtual void updateNodes( const std::vector<void *> & _nodeStates ) = 0;

    virtual void pongByPlayerCatched() = 0;
    virtual void registerInPlayer() = 0;
    virtual void setIdFromPlayer( const TPlayerClientUniqueId & _id ) = 0;
    virtual void updatePlayer( bool _stateHasCome, const SPlayingServiceState & _state ) = 0;

    virtual void addCommmand( PCommand _cmd ) = 0;
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
