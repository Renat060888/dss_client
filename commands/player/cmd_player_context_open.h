#ifndef CMD_CONTEXT_OPEN_H
#define CMD_CONTEXT_OPEN_H

#include "../i_command.h"

namespace dss_client {

class CommandPlayerContextOpen : public ICommand
{
public:
    CommandPlayerContextOpen( common_types::SCommandServices * _commandServices, PNetworkClient _network );

    common_types::TDssClientUniqueId m_clientId;
    std::string m_contextName;

private:
    virtual bool serializeRequestTemplateMethodPart() override;
    virtual bool parseResponseTemplateMethodPart() override;
};
using PCommandPlayerContextOpen = std::shared_ptr<CommandPlayerContextOpen>;

}

#endif // CMD_CONTEXT_OPEN_H
