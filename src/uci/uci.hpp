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

                virtual void resolve_command_sync(const std::string& command_str);
                virtual void resolve_command_async(const std::string& command_str);

            private:
                void handle_uci_command();
                void handle_quit_command();

            private:
                bool m_isReady = false;
                bool m_isInitialized = false;
                bool m_debugMode = false;
        };
    }
};