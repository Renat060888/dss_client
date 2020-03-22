#ifndef COMMON_TYPES_PUBLIC_H
#define COMMON_TYPES_PUBLIC_H

#include <string>

namespace dss_client {

namespace common_types {

// typedefs
using TNodeId = std::string;
using TPid = pid_t;
using TTimeRangeMillisec = std::pair<int64_t, int64_t>;

}

}

#endif // COMMON_TYPES_PUBLIC_H
