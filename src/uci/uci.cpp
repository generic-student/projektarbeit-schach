#include "uci.hpp"
#include "uci_command_builder.hpp"
#include <iostream>
#include <mutex>

namespace sm
{
    namespace uci
    {   
        UniversalChessInterface::UniversalChessInterface()
            : IChessInterface()
        {
        }
        
        void UniversalChessInterface::resolveCommandSync(const std::string& command_str)
        {
            Command command = sm::uci::CommandBuilder::build(command_str);

            switch (command.getType())
            {
            case Command::UCI:
                handleUciCommand();
                break;
            case Command::QUIT:
                handleQuitCommand();
                break;
            case Command::INVALID:
                std::cout << "invalid command!" << std::endl;
                break;
            default:
                std::cout << "command not implemented yet" << std::endl;
                break;
            }
        }
        
        void UniversalChessInterface::resolveCommandAsync(const std::string& command_str)
        {
            throw "Error: Method not implemented!";
        }
        
        void UniversalChessInterface::handleUciCommand()
        {
            std::cout << "id name " << m_pEngine->getID() << std::endl;
            std::cout << "id author " << m_pEngine->getAuthor() << std::endl;

            //TODO: send the settings which the engine supports via the options command

            std::cout << "uciok" << std::endl;
        }
        
        void UniversalChessInterface::handleQuitCommand()
        {
            m_mutex.lock();
            m_isRunning = false;
            //cancle all running calculations
            m_mutex.unlock();

            std::cout << "quit ok" << std::endl;
        }
    }
}