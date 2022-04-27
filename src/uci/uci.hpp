#pragma once
#include <string>
#include "../chess_interface.hpp"

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
                void handleUciCommand();
                void handleDebugCommand();
                void handleIsReadyCommand();
                void handleSetOptionCommand();
                void handlePositionCommand();
                void handleQuitCommand();
                void handleGoCommand();
                void handleStopCommand();
                void handlePonderhitCommand();
                void handleRegisterCommand();
                void handleUciNewGameCommand();

            private:
                bool m_isReady = false;
                bool m_isInitialized = false;
                bool m_debugMode = false;
        };
    }
};