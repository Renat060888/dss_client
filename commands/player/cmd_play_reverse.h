#ifndef CMD_PLAY_REVERSE_H
#define CMD_PLAY_REVERSE_H

#include "../i_command.h"

namespace dss_client{

class CommandPlayReverse : public ICommand {
public:
    CommandPlayReverse( common_types::SCommandServices * _commandServices, PNetworkClient _network );
    ~CommandPlayReverse();

    common_types::TPlayerClientUniqueId m_userIdToPlayer;
    bool reverse;


private:
    virtual bool serializeRequestTemplateMethodPart() override;
    virtual bool parseResponseTemplateMethodPart() override;




};
using PCommandPlayReverse = std::shared_ptr<CommandPlayReverse>;

}

#endif // CMD_PLAY_REVERSE_H
