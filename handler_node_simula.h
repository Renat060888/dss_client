#ifndef HANDLER_NODE_SIMULA_H
#define HANDLER_NODE_SIMULA_H

#include <memory>

#include "common_types_public.h"

namespace dss_client {

class HandlerNodeSimula
{
public:
    // state
    struct SConfigSettings {
        SConfigSettings()
            : simulationIntervalMilllisec(0)
        {}
        int64_t simulationIntervalMilllisec;
    };

    enum class ESimulaNodeStatus {
        READY,
        STARTED,
        PAUSED,
        STOPPED,
        DONE,
        CRASHED,
        UNDEFINED
    };

    struct SState {
        SState()
            : status(ESimulaNodeStatus::UNDEFINED)
            , currentSimulationTimeStepMillisec(0)
        {}
        std::string id;
        ESimulaNodeStatus status;
        int64_t currentSimulationTimeStepMillisec;
        std::pair<int64_t, int64_t> totalSimulationTimeRangeMillisec;
        std::string lastError;
        SConfigSettings currentConfig;
    };

    HandlerNodeSimula();
    ~HandlerNodeSimula();

    const SState & getState();

    // control
    bool configure( SConfigSettings _config );

    bool start();
    bool pause();
    bool reset();


private:
    // private stuff
    class PrivateImplementation * m_impl;

};
using PHandlerNodeSimula = std::shared_ptr<HandlerNodeSimula>;

}

#endif // HANDLER_NODE_SIMULA_H
