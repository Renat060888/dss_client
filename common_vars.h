#ifndef COMMON_VARS_H
#define COMMON_VARS_H

#include "common_types_private.h"

namespace dss_client {

static constexpr const char * PRINT_HEADER = "DssClient:";

namespace common_vars {
    static const common_types::TDssClientUniqueId INVALID_CLIENT_ID = "invalid_client_id";

    // player reponse codes
    static const std::string PLAYER_CODE_NOT_REGISTERED = "NOT_REGISTERED";

    // dss reponse codes
    static const std::string DSS_CODE_NOT_REGISTERED = "NOT_REGISTERED";
}

}

#endif // COMMON_VARS_H
