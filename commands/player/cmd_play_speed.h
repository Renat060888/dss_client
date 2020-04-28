#ifndef CMD_PLAY_SPEED_H
#define CMD_PLAY_SPEED_H

#include "../i_command.h"

namespace dss_client{

class CommandPlaySpeed : public ICommand {
public:
    CommandPlaySpeed( common_types::SCommandServices * _commandServices, PNetworkClient _network );
    ~CommandPlaySpeed();

    common_types::TPlayerClientUniqueId m_userIdToPlayer;
    bool increase;
    bool normalize;


private:
    virtual bool serializeRequestTemplateMethodPart() override;
    virtual bool parseResponseTemplateMethodPart() override;


};
using PCommandPlaySpeed = std::shared_ptr<CommandPlaySpeed>;

}

#endif // CMD_PLAY_SPEED_H
