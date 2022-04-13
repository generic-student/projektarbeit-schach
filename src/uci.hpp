#pragma once
#include <string>

namespace api::uci {
    bool resolve_uci_command(const std::string& command, std::string& out_command);
};