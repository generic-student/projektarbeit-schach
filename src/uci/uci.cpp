#include "uci.hpp"
#include "uci_command_builder.hpp"
#include <iostream>
#include <mutex>
#include "../chess_helper.hpp"

namespace sm
{
    namespace uci
    {
        /**
         * @brief Construct a new UniversalChessInterface::UniversalChessInterface object
         * 
         */
        UniversalChessInterface::UniversalChessInterface()
            : IChessInterface()
        {
        }

        /**
         * @brief resolve a given command in string format
         * 
         * @param command_str uci command
         */
        void UniversalChessInterface::resolveCommand(const std::string &command_str)
        {
            Command command = sm::uci::CommandBuilder::build(command_str);

            switch (command.getType())
            {
            case Command::UCI:
                handleUciCommand(command);
                break;
            case Command::UCI_DEBUG:
                handleDebugCommand(command);
                break;
            case Command::ISREADY:
                handleIsReadyCommand(command);
                break;
            case Command::SETOPTION:
                handleSetOptionCommand(command);
                break;
            case Command::POSITION:
                handlePositionCommand(command);
                break;
            case Command::QUIT:
                handleQuitCommand(command);
                break;
            case Command::GO:
                handleGoCommand(command);
                break;
            case Command::STOP:
                handleStopCommand(command);
                break;
            case Command::PONDERHIT:
                handlePonderhitCommand(command);
                break;
            case Command::REGISTER:
                handleRegisterCommand(command);
                break;
            case Command::UCINEWGAME:
                handleUciNewGameCommand(command);
                break;
            case Command::INVALID:
            default:
                std::cout << "invalid command!" << std::endl;
                break;
            }
        }

        /**
         * @brief handle the "uci" uci-command
         * 
         * @param cmd 
         */
        void UniversalChessInterface::handleUciCommand(Command &cmd)
        {
            std::cout << "id name " << m_pEngine->getID() << std::endl;
            std::cout << "id author " << m_pEngine->getAuthor() << std::endl;

            // TODO: send the settings which the engine supports via the options command

            std::cout << "uciok" << std::endl;
        }

        /**
         * @brief handle the "debug" uci-command
         * 
         * @param cmd 
         */
        void UniversalChessInterface::handleDebugCommand(Command &cmd)
        {
            if (cmd.getArgs().front() == "on")
            {
                m_pEngine->setDebugMode(true);
            }
            else if (cmd.getArgs().front() == "off")
            {
                m_pEngine->setDebugMode(false);
            }
            else
            {
                throw "UCI Exception: debug can either be on or off";
            }
        }

        /**
         * @brief handle the "isready" uci-command
         * 
         * @param cmd 
         */
        void UniversalChessInterface::handleIsReadyCommand(Command &cmd)
        {
        //TODO: Check what this function should actually do
            //check if the engine is currently doing a calculation
            if(m_pEngine->isReady()) {
                std::cout << "readyok" << std::endl;
            }
        }

        /**
         * @brief handle the "setoption" uci-command
         * 
         * @param cmd 
         */
        void UniversalChessInterface::handleSetOptionCommand(Command &cmd)
        {
            const std::string& name = cmd.getArgs()[0];
            const std::string& value = cmd.getArgs()[1];

            m_pEngine->setOption(name, value);
        }

        /**
         * @brief handle the "position" uci-command
         * 
         * @param cmd 
         */
        void UniversalChessInterface::handlePositionCommand(Command &cmd)
        {
            const std::string& pos = cmd.getArgs()[0];
            bool fromStart = (pos == "startpos");

            std::string moves_str = cmd.getArgs()[1];
            std::vector<std::string> moves;
            size_t idx_move;
            while((idx_move = moves_str.find(" ")) != std::string::npos) {
                moves.push_back(moves_str.substr(0, idx_move));
                moves_str.erase(0, idx_move+1);
            }
            moves.push_back(moves_str);

            //set the position of the board to "pos"
            Chessposition& chessposition = m_pEngine->getPosition();
            if(fromStart) {
                chessposition.setFEN(Chessposition::STARTPOS_FEN);
            } else {
                chessposition.setFEN(pos);
            }

            ChessHelper::drawPositionInTerminal(chessposition.getPosition());

            //play the given moves
            std::cout << "evalutated fenstring: " << m_pEngine->evaluateBoard(chessposition.getPosition()) << std::endl;

            std::cout << "command not fully implemented yet" << std::endl;
        }

        /**
         * @brief handle the "quit" uci-command
         * 
         * @param cmd 
         */
        void UniversalChessInterface::handleQuitCommand(Command &cmd)
        {
            m_mutex.lock();
            m_isRunning = false;
            // cancle all running calculations
            m_mutex.unlock();

            std::cout << "quit ok" << std::endl;
        }

        /**
         * @brief handle the "go" uci-command
         * 
         * @param cmd 
         */
        void UniversalChessInterface::handleGoCommand(Command &cmd)
        {
            GoSubcommandData subcommandData;
            const std::vector<std::string>&  args = cmd.getArgs();
            for(const std::string& arg : args) {

            }

            std::cout << "command not fully implemented yet" << std::endl;
        }
        
        /**
         * @brief handle the subcommands that follow the "go" uci-command
         * 
         * @param subcommand 
         */
        void UniversalChessInterface::handleGoSubcommand(const std::string& subcommand)
        {
            
        }

        /**
         * @brief handle the "stop" uci-command
         * 
         * @param cmd 
         */
        void UniversalChessInterface::handleStopCommand(Command &cmd)
        {
            std::cout << "command not implemented yet" << std::endl;
        }

        /**
         * @brief handle the "ponderhit" uci-command
         * 
         * @param cmd 
         */
        void UniversalChessInterface::handlePonderhitCommand(Command &cmd)
        {
            std::cout << "command not implemented yet" << std::endl;
        }

        /**
         * @brief handle the "register" uci-command
         * 
         * @param cmd 
         */
        void UniversalChessInterface::handleRegisterCommand(Command &cmd)
        {
            std::cout << "command not implemented yet" << std::endl;
        }

        /**
         * @brief handle the "ucinewgame" uci-command
         * 
         * @param cmd 
         */
        void UniversalChessInterface::handleUciNewGameCommand(Command &cmd)
        {
            std::cout << "command not implemented yet" << std::endl;
        }
    }
}