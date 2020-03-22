#ifndef CMD_PLAY_STEP_BACK_H
#define CMD_PLAY_STEP_BACK_H

#include "../i_command.h"

namespace dss_client{

class CommandPlayStep : public ICommand {
public:
    CommandPlayStep( common_types::SCommandServices * _commandServices, PNetworkClient _network );
    ~CommandPlayStep();

    bool stepForward;


private:
    virtual bool serializeRequestTemplateMethodPart() override;
    virtual bool parseResponseTemplateMethodPart() override;



};
using PCommandPlayStep = std::shared_ptr<CommandPlayStep>;

}

#endif // CMD_PLAY_STEP_BACK_H
