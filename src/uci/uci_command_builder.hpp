#pragma once
#include <string>
#include "uci_command.hpp"

namespace engine::uci
{
    class CommandBuilder
    {
    public:
        enum class CommandType
        {
            UCI,
            DEBUG,
            ISREADY,
            SETOPTION,
            POSITION,
            GO_PONDER,
            GO_WTIME,
            GO_BTIME,
            GO_WINC,
            GO_BINC,
            GO_MOVESTOGO,
            GO_DEPTH,
            GO_NODES,
            GO_MATE,
            GO_MOVETIME,
            GO_INFINITE,
            STOP,
            PONDERHIT,
            QUIT
        };
    public:
        static Command fromString(const std::string& command_str);

    private:
        explicit CommandBuilder() = default;
        virtual ~CommandBuilder() = default;
        

    };
}