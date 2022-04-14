#include "uci_command_builder.hpp"
#include <unordered_map>
#include <regex>

namespace sm::uci
{
    const std::unordered_map<const Command::Type, const std::string> COMMAND_VALIDATION_MAP = {
        {Command::Type::UCI, "^(uci)$"},
        {Command::Type::DEBUG, "^(debug) (on|off)$"},
        {Command::Type::ISREADY, "^(isready)$"},
        {Command::Type::SETOPTION, "^(setoption) name (.*?)(\\svalue .*)?$"},
        {Command::Type::POSITION, "^(position)\\s?(.*)?$"}
    };

    Command CommandBuilder::fromString(const std::string& command_str)
    {        
        for(const auto it : COMMAND_VALIDATION_MAP) {
            if(std::regex_match(command_str, std::regex(it.second))) {            
                return Command(it.first, {});
            }
        }

        //the command is not in the list of valid commands
        return Command(Command::Type::INVALID, {});
    }
}