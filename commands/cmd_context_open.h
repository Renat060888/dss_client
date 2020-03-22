#ifndef CMD_CONTEXT_OPEN_H
#define CMD_CONTEXT_OPEN_H

#include "i_command.h"

namespace dss_client {

class CommandContextOpen : public ICommand
{
public:
    CommandContextOpen( common_types::SCommandServices * _commandServices, PNetworkClient _network );

    common_types::TDssClientUniqueId m_clientId;
    std::string m_contextName;

private:
    virtual bool serializeRequestTemplateMethodPart() override;
    virtual bool parseResponseTemplateMethodPart() override;
};
using PCommandContextOpen = std::shared_ptr<CommandContextOpen>;

}

#endif // CMD_CONTEXT_OPEN_H
