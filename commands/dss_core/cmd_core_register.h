#ifndef CMD_CORE_REGISTER_H
#define CMD_CORE_REGISTER_H

#include "../i_command.h"

namespace dss_client {

class CommandRegister : public ICommand
{
public:
    CommandRegister( common_types::SCommandServices * _commandServices, PNetworkClient _network );

    std::string m_userIpStr;
    common_types::TPid m_userPid;

private:
    virtual bool serializeRequestTemplateMethodPart() override;
    virtual bool parseResponseTemplateMethodPart() override;
};
using PCommandRegister = std::shared_ptr<CommandRegister>;

}

#endif // CMD_CORE_REGISTER_H
