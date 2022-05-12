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

            const std::vector<std::string>& getArgs() const;
            void setArgs(const std::vector<std::string>& args);

        private:
            Command::Type m_type = Command::Type::INVALID;
            std::vector<std::string> m_args;
        };

        /**
         * @brief All possible values that can be sent alongside the go command.
         * If an integer value is set to 0 then it has not been set by the go command => it is undefined and should not be used.
         * If the searchmoves are empty, they have not been set and should not be used.
         */
        struct GoSubcommandData {
            std::vector<std::string> searchmoves;
            bool ponder = false;
            int wtime = 0;
            int btime = 0;
            int winc = 0;
            int binc = 0;
            int movestogo = 0;
            int depth = 0;
            int nodes = 0;
            int mate = 0;
            int movetime = 0;
            bool infinite = false;
        };
    }
}