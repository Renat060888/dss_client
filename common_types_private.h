#ifndef COMMON_TYPES_PRIVATE_H
#define COMMON_TYPES_PRIVATE_H

#include <microservice_common/communication/network_interface.h>

#include "common_types_public.h"

namespace dss_client {

namespace common_types {


// typedefs
using TClientUniqueId = std::string;


// containers
struct SCommandServices {
    PNetworkClient network;
};


}

}

#endif // COMMON_TYPES_PRIVATE_H
