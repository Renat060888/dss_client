#ifndef COMMON_TYPES_PUBLIC_H
#define COMMON_TYPES_PUBLIC_H

#include <string>
#include <vector>

namespace dss_client {

namespace common_types {

// typedefs
using TNodeId = std::string;
using TPid = pid_t;
using TTimeRangeMillisec = std::pair<int64_t, int64_t>;

// -------------------------------------------------------------
// NOTE: must be equal to the same structures in 'PlayerAgent'
// -------------------------------------------------------------
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

struct SPlayingDataSet {
    int uniqueId;
    char description[64];
    bool real;
    std::vector<TTimeRangeMillisec> dataRanges;
};

struct SPlayingInfo {
    TTimeRangeMillisec globalRangeMillisec;
    int64_t currentStepMillisec;
    int64_t stepIntervalMillisec;
    std::vector<SPlayingDataSet> playingData;
};

struct SPlayingServiceState {
//    TContextId ctxId;
//    TPlayerId playerId;
    EPlayerStatus status;
    SPlayingInfo info;
    std::string lastError;
};
// -------------------------------------------------------------
// NOTE: must be equal to the same structures in 'PlayerAgent'
// -------------------------------------------------------------

}

}

#endif // COMMON_TYPES_PUBLIC_H
