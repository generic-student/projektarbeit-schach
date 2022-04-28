#include "chess_interface.hpp"
#include <iostream>

namespace sm
{
    IChessInterface::IChessInterface()
        : m_pEngine(boost::shared_ptr<Engine>(new sm::Engine))
    {
    }

    void IChessInterface::start() {
        m_isRunning = true;
        while(m_isRunning) {
            std::string input;
            std::getline(std::cin, input);

            resolveCommandSync(input);
        }
    }
}