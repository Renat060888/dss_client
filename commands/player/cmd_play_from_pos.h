#ifndef CMD_PLAYER_FROM_POS_H
#define CMD_PLAYER_FROM_POS_H

#include "../i_command.h"

namespace dss_client{

class CommandPlayFromPos : public ICommand {
public:
    CommandPlayFromPos( common_types::SCommandServices * _commandServices, PNetworkClient _network );
    ~CommandPlayFromPos();

    common_types::TPlayerClientUniqueId m_userIdToPlayer;
    int64_t targetPosMillisec;


private:
    virtual bool serializeRequestTemplateMethodPart() override;
    virtual bool parseResponseTemplateMethodPart() override;



};
using PCommandPlayFromPos = std::shared_ptr<CommandPlayFromPos>;

}

#endif // CMD_PLAYER_FROM_POS_H
