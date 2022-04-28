#pragma once
#include <string>
#include <boost/shared_ptr.hpp>
#include "engine.hpp"
#include <mutex>

namespace sm {
    /**
     * @brief Interface class that defines the basic functionality of a chess interface
     * 
     */
    class IChessInterface {
        public:
            explicit IChessInterface();
            virtual ~IChessInterface() = default;
            void start();

            virtual void resolveCommand(const std::string& command_str) = 0;

        protected:
            bool m_isRunning = false;
            boost::shared_ptr<Engine> m_pEngine;
            std::mutex m_mutex;
    };
}