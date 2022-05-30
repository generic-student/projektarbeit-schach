#pragma once
#include <string>
#include "../chess_interface.hpp"
#include "uci_command.hpp"
#include <memory>
#include <thread>

namespace sm
{
    namespace uci
    {
        class UniversalChessInterface : public IChessInterface {
            public:
                explicit UniversalChessInterface();

                virtual void resolveCommand(const std::string& command_str);

            private:
                void handleUciCommand(Command& cmd);
                void handleDebugCommand(Command& cmd);
                void handleIsReadyCommand(Command& cmd);
                void handleSetOptionCommand(Command& cmd);
                void handlePositionCommand(Command& cmd);
                void handleQuitCommand(Command& cmd);
                void handleGoCommand(Command& cmd);
                void handleGoSubcommand(const std::string& arg, GoSubcommandData& data);
                void handleStopCommand(Command& cmd);
                void handlePonderhitCommand(Command& cmd);
                void handleRegisterCommand(Command& cmd);
                void handleUciNewGameCommand(Command& cmd);

                void go(std::shared_ptr<GoSubcommandData> p_data);

            private:
                bool m_isReady = false;
                bool m_isInitialized = false;
                bool m_debugMode = false;
                bool m_readyCheckQueued = false;

                std::thread m_minmaxThread;
        };
    }
};