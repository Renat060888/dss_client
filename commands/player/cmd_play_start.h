#ifndef CMD_PLAY_START_H
#define CMD_PLAY_START_H

#include "../i_command.h"

namespace dss_client{

class CommandPlayStart : public ICommand {    
public:
    CommandPlayStart( common_types::SCommandServices * _commandServices, PNetworkClient _network );
    ~CommandPlayStart();

    common_types::TPlayerClientUniqueId m_userIdToPlayer;

private:
    virtual bool serializeRequestTemplateMethodPart() override;
    virtual bool parseResponseTemplateMethodPart() override;


};
using PCommandPlayStart = std::shared_ptr<CommandPlayStart>;

}

#endif // CMD_PLAY_START_H
