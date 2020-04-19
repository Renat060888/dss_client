#ifndef CMD_CORE_PING_H
#define CMD_CORE_PING_H

#include "../i_command.h"

namespace dss_client {

class CommandDSSPing : public ICommand
{
public:
    CommandDSSPing( common_types::SCommandServices * _commandServices, PNetworkClient _network );

    common_types::TDssClientUniqueId m_userIdToDSS;

private:
    virtual bool serializeRequestTemplateMethodPart() override;
    virtual bool parseResponseTemplateMethodPart() override;
};
using PCommandDSSPing = std::shared_ptr<CommandDSSPing>;

}

#endif // CMD_CORE_PING_H
