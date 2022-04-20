#pragma once
#include <string>
#include <map>

namespace sm
{
    namespace uci
    {
        class Command
        {
        public:
            enum Type
            {
                UCI,
                UCI_DEBUG,
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
                QUIT,
                INVALID
            };

        public:
            explicit Command(const Command::Type type, const std::map<std::string, std::string> &args);
            explicit Command() = default;
            virtual ~Command() = default;

            Command::Type getType() const;
            void setType(Command::Type type);

        private:
            Command::Type m_type = Command::Type::INVALID;
            std::map<std::string, std::string> m_args;
        };
    }
}