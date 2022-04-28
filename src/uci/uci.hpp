#pragma once
#include <string>
#include "../chess_interface.hpp"
#include "uci_command.hpp"

namespace sm
{
    namespace uci
    {
        class UniversalChessInterface : public IChessInterface {
            public:
                explicit UniversalChessInterface();

                virtual void resolveCommandSync(const std::string& command_str);
                virtual void resolveCommandAsync(const std::string& command_str);

            private:
                void handleUciCommand(Command& cmd);
                void handleDebugCommand(Command& cmd);
                void handleIsReadyCommand(Command& cmd);
                void handleSetOptionCommand(Command& cmd);
                void handlePositionCommand(Command& cmd);
                void handleQuitCommand(Command& cmd);
                void handleGoCommand(Command& cmd);
                void handleGoSubcommand(const std::string& subcommand);
                void handleStopCommand(Command& cmd);
                void handlePonderhitCommand(Command& cmd);
                void handleRegisterCommand(Command& cmd);
                void handleUciNewGameCommand(Command& cmd);

            private:
                bool m_isReady = false;
                bool m_isInitialized = false;
                bool m_debugMode = false;
        };
    }
};