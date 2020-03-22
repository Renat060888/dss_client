#ifndef CMD_CONTEXT_CLOSE_H
#define CMD_CONTEXT_CLOSE_H

#include "i_command.h"

namespace dss_client {

class CommandContextClose : public ICommand
{
public:
    CommandContextClose( common_types::SCommandServices * _commandServices, PNetworkClient _network );

    common_types::TDssClientUniqueId m_clientId;

private:
    virtual bool serializeRequestTemplateMethodPart() override;
    virtual bool parseResponseTemplateMethodPart() override;
};
using PCommandContextClose = std::shared_ptr<CommandContextClose>;

}


#endif // CMD_CONTEXT_CLOSE_H
