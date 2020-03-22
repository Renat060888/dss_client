#ifndef CMD_PLAY_LOOP_H
#define CMD_PLAY_LOOP_H

#include "../i_command.h"

namespace dss_client{

class CommandPlayLoop : public ICommand {
public:
    CommandPlayLoop( common_types::SCommandServices * _commandServices, PNetworkClient _network );
    ~CommandPlayLoop();

    bool loop;

private:
    virtual bool serializeRequestTemplateMethodPart() override;
    virtual bool parseResponseTemplateMethodPart() override;




};
using PCommandPlayLoop = std::shared_ptr<CommandPlayLoop>;

}

#endif // CMD_PLAY_LOOP_H
