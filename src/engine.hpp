#pragma once
#include "uci/uci_interface.hpp"
#include <string>

namespace engine {
    class Engine {
        public:
            explicit Engine();
            virtual ~Engine() = default;

            void start();

        private:
            bool m_isRunning = false;
            bool m_isReady = false;
            bool m_isInitialized = false;

            uci::UciInterface m_interface;
    };
}