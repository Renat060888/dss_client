
#include "cmd_node_start.h"

using namespace std;

namespace dss_client{

using namespace common_types;

CommandNodeStart::CommandNodeStart( SCommandServices * _commandServices, PNetworkClient _network )
    : ICommand(_commandServices, _network)
{

}

bool CommandNodeStart::serializeRequestTemplateMethodPart(){

}

bool CommandNodeStart::parseResponseTemplateMethodPart(){

}

}

