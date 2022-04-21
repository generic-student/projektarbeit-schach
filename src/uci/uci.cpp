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
        
        void UniversalChessInterface::resolve_command_sync(const std::string& command_str)
        {
            Command command = sm::uci::CommandBuilder::build(command_str);

            switch (command.getType())
            {
            case Command::UCI:
                handle_uci_command();
                break;
            case Command::QUIT:
                handle_quit_command();
                break;
            case Command::INVALID:
                std::cout << "invalid command!" << std::endl;
                break;
            default:
                std::cout << "command not implemented yet" << std::endl;
                break;
            }
        }
        
        void UniversalChessInterface::resolve_command_async(const std::string& command_str)
        {
            throw "Error: Method not implemented!";
        }
        
        void UniversalChessInterface::handle_uci_command()
        {
            std::cout << "id name " << m_pEngine->getID() << std::endl;
            std::cout << "id author " << m_pEngine->getAuthor() << std::endl;

            //TODO: send the settings which the engine supports via the options command

            std::cout << "uciok" << std::endl;
        }
        
        void UniversalChessInterface::handle_quit_command()
        {
            m_mutex.lock();
            m_isRunning = false;
            //cancle all running calculations
            m_mutex.unlock();

            std::cout << "quit ok" << std::endl;
        }
    }
}