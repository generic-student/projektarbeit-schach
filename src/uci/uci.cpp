#include "uci.hpp"
#include "uci_command_builder.hpp"
#include <iostream>
#include <mutex>
#include <regex>
#include "../chess_helper.hpp"

#include <spdlog/spdlog.h>

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
            case Command::EVALUATE:
                handleEvaluateCommand(command);
                break;
            case Command::GETVALIDMOVES:
                handleGetValidMovesCommand(command);
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

            // send the settings which the engine supports via the options command
            const auto &options = m_pEngine->getOptions().getEngineOptions();
            for (const auto &it : options)
            {
                if (!it.second.isSupported)
                    continue;

                std::cout << "option name " << it.first << " type " << it.second.type << " default " << it.second.default_value << std::endl;
            }

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
            // TODO: Check what this function should actually do
            // check if the engine is currently doing a calculation
            if (m_pEngine->isReady())
            {
                std::cout << "readyok" << std::endl;
            } else {
                m_mutex.lock();
                m_readyCheckQueued = true;
                m_mutex.unlock();
            }
        }

        /**
         * @brief handle the "setoption" uci-command
         *
         * @param cmd
         */
        void UniversalChessInterface::handleSetOptionCommand(Command &cmd)
        {
            const std::string &name = cmd.getArgs()[0];
            const std::string &value = cmd.getArgs()[1];

            m_pEngine->setOption(name, value);
        }

        /**
         * @brief handle the "position" uci-command
         *
         * @param cmd
         */
        void UniversalChessInterface::handlePositionCommand(Command &cmd)
        {
            const std::string &pos = cmd.getArgs()[0];
            bool fromStart = (pos == "startpos");

            std::string moves_str = cmd.getArgs()[1];
            std::vector<std::string> moves;
            size_t idx_move;
            while ((idx_move = moves_str.find(" ")) != std::string::npos)
            {
                moves.push_back(moves_str.substr(0, idx_move));
                moves_str.erase(0, idx_move + 1);
            }
            moves.push_back(moves_str);

            // set the position of the board to "pos"
            Chessposition &chessposition = m_pEngine->getPosition();
            if (fromStart)
            {
                chessposition.setFEN(Chessposition::STARTPOS_FEN);
                chessposition.setActivePlayer(Chessposition::Player::WHITE);
            }
            else
            {
                chessposition.setFEN(pos);
            }

            for (auto &move_str : moves)
            {
                Move move = ChessHelper::parseMove(move_str);
                if (chessposition.getActivePlayer() == Chessposition::Player::WHITE && move.promotion != '\0')
                {
                    move.promotion -= 32;
                }

                if (move_str != "0000" && !m_pEngine->getPosition().applyMove(move, false))
                {
                    spdlog::warn("invalid move");
                    std::cout << "Invalid Move" << std::endl;
                    break;
                }
                spdlog::info("played move: " + move_str);
            }

            // ChessHelper::drawPositionInTerminal(chessposition.getPosition());

            // play the given moves
            // std::cout << "evalutated fenstring: " << m_pEngine->evaluateBoard(chessposition.getPosition()) << std::endl;

            // std::cout << "command not fully implemented yet" << std::endl;
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
            m_mutex.unlock();

            this->handleStopCommand(cmd);

            std::cout << "quit ok" << std::endl;
        }

        /**
         * @brief handle the "go" uci-command
         *
         * @param cmd
         */
        void UniversalChessInterface::handleGoCommand(Command &cmd)
        {
            std::shared_ptr<GoSubcommandData> subcommandData = std::make_shared<GoSubcommandData>();
            const std::vector<std::string> &args = cmd.getArgs();
            for (const std::string &arg : args)
            {
                handleGoSubcommand(arg, *subcommandData);
            }

            if(m_pEngine->isReady()) {
                if(m_minmaxThread.joinable()) {
                    m_minmaxThread.join();
                }
            }

            this->m_minmaxThread = std::thread(&UniversalChessInterface::go, this, subcommandData);
        }

        /**
         * @brief handle the subcommands that follow the "go" uci-command
         *
         * @param subcommand
         */
        void UniversalChessInterface::handleGoSubcommand(const std::string &arg, GoSubcommandData &data)
        {
            // first the boolean flags
            if (arg == "ponder")
            {
                data.ponder = true;
                return;
            }

            if (arg == "infinite")
            {
                data.infinite = true;
                return;
            }

            std::smatch match;

            // check if it is the searchmoves command since it is the only command left that doesnt have an integer value
            if (arg.find("searchmoves") != std::string::npos)
            {
                if (std::regex_search(arg, match, std::regex("([a-hA-H0-8x\\+]{4,6})")))
                {
                    for (size_t i = 1; i < match.size(); i++)
                    {
                        if (match.str(i).empty())
                            continue;

                        data.searchmoves.push_back(match.str(i));
                    }
                }
                return;
            }

            // all subcommands that are left have an integer value
            // get the value using regex
            int value;
            if (std::regex_search(arg, match, std::regex("-?\\d+")))
            {
                value = atoi(match.str(0).data());
            }
            else
            {
                return;
            }

            if (arg.find("wtime") != std::string::npos)
            {
                data.wtime = value;
                return;
            }

            if (arg.find("btime") != std::string::npos)
            {
                data.btime = value;
                return;
            }

            if (arg.find("winc") != std::string::npos)
            {
                data.winc = value;
                return;
            }

            if (arg.find("binc") != std::string::npos)
            {
                data.binc = value;
                return;
            }

            if (arg.find("movestogo") != std::string::npos)
            {
                data.movestogo = value;
                return;
            }

            if (arg.find("depth") != std::string::npos)
            {
                data.depth = value;
                return;
            }

            if (arg.find("nodes") != std::string::npos)
            {
                data.nodes = value;
                return;
            }

            if (arg.find("mate") != std::string::npos)
            {
                data.mate = value;
                return;
            }

            if (arg.find("movetime") != std::string::npos)
            {
                data.movetime = value;
                return;
            }
        }

        /**
         * @brief handle the "stop" uci-command
         *
         * @param cmd
         */
        void UniversalChessInterface::handleStopCommand(Command &cmd)
        {
            m_pEngine->stop();

            if(m_minmaxThread.joinable()) {
                m_minmaxThread.join();
            }

            //std::cout << "command not implemented yet" << std::endl;
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
            m_pEngine->getPosition() = Chessposition(Chessposition::STARTPOS_FEN);
        }
        
        void UniversalChessInterface::handleEvaluateCommand(Command& cmd)
        {
            std::cout << m_pEngine->evaluateBoard(m_pEngine->getPosition()) << std::endl;
        }
        
        void UniversalChessInterface::handleGetValidMovesCommand(Command& cmd)
        {
            auto moves = m_pEngine->getPosition().getValidMoves();
            for(size_t i = 0; i < moves.size() - 1; ++i) {
                std::cout << ChessHelper::moveToString(moves[i]) << ",";
            }
            if(!moves.empty())
                std::cout << ChessHelper::moveToString(moves.back()) << std::endl;
            else
                std::cout << "null" << std::endl; 
        }
        
        void UniversalChessInterface::go(std::shared_ptr<GoSubcommandData> p_data)
        {
            const Chessposition& pos = m_pEngine->getPosition();
            const Chessposition::Player player = pos.getActivePlayer();

            int depth = p_data->depth == 0 ? 5 : p_data->depth;

            MinMaxResult result = m_pEngine->findMove(pos, player, depth);

            std::cout << "bestmove " << ChessHelper::moveToString(result.move) << std::endl;
            //spdlog::info("bestmove " + ChessHelper::moveToString(result.move));

            if(m_readyCheckQueued) {
                std::cout << "readyok" << std::endl;
                m_mutex.lock();
                m_readyCheckQueued = false;
                m_mutex.unlock();
            }
        }
    }
}