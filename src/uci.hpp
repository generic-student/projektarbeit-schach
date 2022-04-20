#pragma once
#include <string>

namespace api
{
    namespace uci
    {
        bool resolve_uci_command(const std::string &command, std::string &out_command);
    }
};