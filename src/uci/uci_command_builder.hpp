#pragma once
#include <string>
#include "uci_command.hpp"

namespace sm
{
    namespace uci
    {
        class CommandBuilder
        {
        public:
            static Command fromString(const std::string &command_str);

        private:
            explicit CommandBuilder() = default;
            virtual ~CommandBuilder() = default;
        };
    }
}