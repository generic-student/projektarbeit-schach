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
            case Command::UCI_DEBUG:
                handleDebugCommand();
                break;
            case Command::ISREADY:
                handleIsReadyCommand();
                break;
            case Command::SETOPTION:
                handleSetOptionCommand();
                break;
            case Command::POSITION:
                handlePositionCommand();
                break;
            case Command::QUIT:
                handleQuitCommand();
                break;
            case Command::GO:
                handleGoCommand();
                break;
            case Command::STOP:
                handleStopCommand();
                break;
            case Command::PONDERHIT:
                handlePonderhitCommand();
                break;
            case Command::REGISTER:
                handleRegisterCommand();
                break;
            case Command::UCINEWGAME:
                handleUciNewGameCommand();
                break;
            case Command::INVALID:
            default:
                std::cout << "invalid command!" << std::endl;
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
        
        void UniversalChessInterface::handleDebugCommand()
        {
            std::cout << "command not implemented yet" << std::endl;
        }
        
        void UniversalChessInterface::handleIsReadyCommand()
        {
            std::cout << "command not implemented yet" << std::endl;
        }
        
        void UniversalChessInterface::handleSetOptionCommand()
        {
            std::cout << "command not implemented yet" << std::endl;
        }
        
        void UniversalChessInterface::handlePositionCommand()
        {
            std::cout << "command not implemented yet" << std::endl;
        }
        
        void UniversalChessInterface::handleQuitCommand()
        {
            m_mutex.lock();
            m_isRunning = false;
            //cancle all running calculations
            m_mutex.unlock();

            std::cout << "quit ok" << std::endl;
        }
        
        void UniversalChessInterface::handleGoCommand()
        {
            std::cout << "command not implemented yet" << std::endl;
        }
        
        void UniversalChessInterface::handleStopCommand()
        {
            std::cout << "command not implemented yet" << std::endl;
        }
        
        void UniversalChessInterface::handlePonderhitCommand()
        {
            std::cout << "command not implemented yet" << std::endl;
        }
        
        void UniversalChessInterface::handleRegisterCommand()
        {
            std::cout << "command not implemented yet" << std::endl;
        }
        
        void UniversalChessInterface::handleUciNewGameCommand()
        {
            std::cout << "command not implemented yet" << std::endl;
        }
    }
}