#include "chess_interface.hpp"
#include <iostream>

namespace sm
{
    IChessInterface::IChessInterface()
        : m_pEngine(std::make_shared<Engine>())
    {
    }

    /**
     * @brief Starts the Chessinterface. The chess interface will read input from stdin and process it according to the 
     * virtual function void IChessInterface::resolveCommand(const std::string& command_str) that will be overwritten by
     * the inheriting class.
     */
    void IChessInterface::start() {
        m_isRunning = true;
        while(m_isRunning) {
            std::string input;
            std::getline(std::cin, input);

            resolveCommand(input);
        }
    }
}