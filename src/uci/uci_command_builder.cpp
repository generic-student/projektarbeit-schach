#include "uci_command_builder.hpp"
#include <array>
#include <regex>

namespace sm
{
    namespace uci
    {
        const std::array<const std::string, 6U> COMMAND_VALIDATION_MAP = {
            "^(uci)$",
            "^(debug) (on|off)$",
            "^(isready)$",
            "^(setoption) name (.*?)(\\svalue .*)?$",
            "^(position)\\s?(.*)?$",
            "^(quit)$"
        };

        Command CommandBuilder::build(const std::string &command_str)
        {
            for(size_t i = 0; i < COMMAND_VALIDATION_MAP.size(); i++) {
                if (std::regex_match(command_str, std::regex(COMMAND_VALIDATION_MAP[i])))
                {
                    return Command((Command::Type)i, {});
                }
            }

            // the command is not in the list of valid commands
            return Command(Command::Type::INVALID, {});
        }
    }
}