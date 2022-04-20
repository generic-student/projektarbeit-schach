#include "uci.hpp"

namespace sm
{
    namespace uci
    {
        /**
         * @brief check if a given command adheres to the uci spec
         *
         * @param command the command to be validated
         * @return true if the command is valid
         * @return false if the command is invalid
         */
        bool is_valid_uci_command(const std::string &command)
        {
            return false;
        }

        /**
         * @brief resolve the incoming uci-command if it is valid
         *
         * @param command a uci-command
         * @param out_command the result of the command
         * @return true if the incoming command is valid and could be resolved
         * @return false if the incoming command is invalid and could not be resolved
         */
        bool resolve_uci_command(const std::string &command, std::string &out_command)
        {
            return false;
        }
    }
}