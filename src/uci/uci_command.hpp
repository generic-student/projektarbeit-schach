#pragma once
#include <string>
#include <vector>

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
                QUIT,
                GO,
                STOP,
                PONDERHIT,
                REGISTER,
                UCINEWGAME,
                INVALID
            };

        public:
            explicit Command(const Command::Type type, const std::vector<std::string> &args);
            explicit Command() = default;
            virtual ~Command() = default;

            Command::Type getType() const;
            void setType(Command::Type type);

        private:
            Command::Type m_type = Command::Type::INVALID;
            std::vector<std::string> m_args;
        };
    }
}