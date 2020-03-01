#ifndef CMD_PING_H
#define CMD_PING_H

#include "i_command.h"

namespace dss_client {

class CommandPing : public ICommand
{
public:
    CommandPing( common_types::SCommandServices * _commandServices );

    common_types::TClientUniqueId m_userId;

private:
    virtual bool serializeRequestTemplateMethodPart() override;
    virtual bool parseResponseTemplateMethodPart() override;
};
using PCommandPing = std::shared_ptr<CommandPing>;

}

#endif // CMD_PING_H
