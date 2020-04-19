#ifndef CMD_REGISTER_H
#define CMD_REGISTER_H

#include "../i_command.h"

namespace dss_client {

class CommandPlayerRegister : public ICommand
{
public:
    CommandPlayerRegister( common_types::SCommandServices * _commandServices, PNetworkClient _network );

    std::string m_userIpStr;
    common_types::TPid m_userPid;

private:
    virtual bool serializeRequestTemplateMethodPart() override;
    virtual bool parseResponseTemplateMethodPart() override;
};
using PCommandPlayerRegister = std::shared_ptr<CommandPlayerRegister>;

}

#endif // CMD_REGISTER_H
