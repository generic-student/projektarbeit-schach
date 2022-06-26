#pragma once
#include <string>
#include <memory>
#include "engine.hpp"
#include <mutex>

namespace sm {
    /**
     * @brief Interface class that defines the basic functionality of a chess interface
     * 
     */
    class IChessInterface {
        public:
            /**
             * @brief Construct a new IChessInterface::IChessInterface object
             * 
             */
            explicit IChessInterface();
            /**
             * @brief Destroy the IChessInterface object
             * 
             */
            virtual ~IChessInterface() = default;

            /**
             * @brief Starts the interface.
             * The interface will begin to listen for stdin and resolves
             * incoming commands.
             * 
             */
            void start();

            /**
             * @brief Defines how a given string will be resolved.
             * 
             * @param command_str command
             */
            virtual void resolveCommand(const std::string& command_str) = 0;

        protected:
            // Determines if the engine is currently running
            bool m_isRunning = false;
            // An instance of the Chess-Engine
            std::shared_ptr<Engine> m_pEngine;
            // A mutex to take care of thread-safety
            std::mutex m_mutex;
    };
}