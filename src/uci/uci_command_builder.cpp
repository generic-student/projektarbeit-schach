#include "uci_command_builder.hpp"
#include <array>
#include <regex>

namespace sm
{
    namespace uci
    {
        const std::array<const std::string, 11U> COMMAND_VALIDATION_MAP = {
            "^uci$",
            "^debug (on|off)$",
            "^isready$",
            "^setoption name (.*?)(\\svalue .*)?$",
            "^position (startpos|[a-hA-H0-9]+) (moves [a-hA-H0-9\\+]+)$",
            "^quit$",
            "^go(?: (?:(infinite)|(searchmoves [a-hA-H0-8]{4,6})+|(ponder)|(wtime \\d+)|(btime \\d+)|(winc -?\\d+)|(binc -?\\d+)|(movestogo \\d+)|(depth \\d+)|(nodes \\d+)|(mate \\d+)|(movetime \\d+)|(infinite)))+$",
            "^stop$",
            "^ponderhit$",
            "^register (later)|(?:(name .*(?= code))? (code .*)?)$"
        };

        Command CommandBuilder::build(const std::string &command_str)
        {
            std::smatch match;

            for(size_t i = 0; i < COMMAND_VALIDATION_MAP.size(); i++) {
                if (std::regex_search(command_str, match, std::regex(COMMAND_VALIDATION_MAP[i])))
                {
                    std::vector<std::string> args;
                    for(size_t i = 1; i < match.size(); i++) {
                        if(match.str(i).empty()) continue;
                        
                        args.push_back(match.str(i));
                    }

                    return Command((Command::Type)i, args);
                }
            }

            // the command is not in the list of valid commands
            return Command(Command::Type::INVALID, {});
        }
    }
}