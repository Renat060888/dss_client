#ifndef CMD_NODE_START_H
#define CMD_NODE_START_H

#include "../i_command.h"

namespace dss_client {

class CommandNodeStart : public ICommand
{
public:
    CommandNodeStart( common_types::SCommandServices * _commandServices, PNetworkClient _network );

    common_types::TDssClientUniqueId m_userId;

private:
    virtual bool serializeRequestTemplateMethodPart() override;
    virtual bool parseResponseTemplateMethodPart() override;
};
using PCommandNodeStart = std::shared_ptr<CommandNodeStart>;

}

#endif // CMD_NODE_START_H
