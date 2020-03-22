#ifndef CMD_PLAYER_STOP_H
#define CMD_PLAYER_STOP_H

#include "../i_command.h"

namespace dss_client{

class CommandPlayStop : public ICommand {
public:

    CommandPlayStop( common_types::SCommandServices * _commandServices, PNetworkClient _network );
    ~CommandPlayStop();


private:
    virtual bool serializeRequestTemplateMethodPart() override;
    virtual bool parseResponseTemplateMethodPart() override;

};
using PCommandPlayStop = std::shared_ptr<CommandPlayStop>;

}

#endif // CMD_PLAYER_STOP_H
