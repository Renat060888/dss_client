#ifndef CMD_CONTEXT_CLOSE_H
#define CMD_CONTEXT_CLOSE_H

#include "../i_command.h"

namespace dss_client {

class CommandPlayerContextClose : public ICommand
{
public:
    CommandPlayerContextClose( common_types::SCommandServices * _commandServices, PNetworkClient _network );

    common_types::TDssClientUniqueId m_userId;

private:
    virtual bool serializeRequestTemplateMethodPart() override;
    virtual bool parseResponseTemplateMethodPart() override;
};
using PCommandPlayerContextClose = std::shared_ptr<CommandPlayerContextClose>;

}


#endif // CMD_CONTEXT_CLOSE_H
