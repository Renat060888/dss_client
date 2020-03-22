#ifndef CMD_PLAY_PAUSE_H
#define CMD_PLAY_PAUSE_H

#include "../i_command.h"

namespace dss_client{

class CommandPlayPause : public ICommand {
    // TODO: private CTOR/DTOR
    friend class VideoServerClient;
public:
    struct SInitialParams {
        SInitialParams()
        {}
    };

    CommandPlayPause( common_types::SCommandServices * _commandServices, PNetworkClient _network );
    ~CommandPlayPause();

    bool init( SInitialParams _params );

private:
    virtual bool serializeRequestTemplateMethodPart() override;
    virtual bool parseResponseTemplateMethodPart() override;


};
using PCommandPlayPause = std::shared_ptr<CommandPlayPause>;

}

#endif // CMD_PLAY_PAUSE_H
