#ifndef CMD_PLAY_REALTIME_H
#define CMD_PLAY_REALTIME_H

#include "../i_command.h"

namespace dss_client{

class CommandPlayLive : public ICommand {
public:
    CommandPlayLive( common_types::SCommandServices * _commandServices, PNetworkClient _network );
    ~CommandPlayLive();

    common_types::TPlayerClientUniqueId m_userIdToPlayer;
    bool live;


private:
    virtual bool serializeRequestTemplateMethodPart() override;
    virtual bool parseResponseTemplateMethodPart() override;


};
using PCommandPlayLive = std::shared_ptr<CommandPlayLive>;

}

#endif // CMD_PLAY_REALTIME_H
