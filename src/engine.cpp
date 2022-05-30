#include "engine.hpp"
#include <spdlog/spdlog.h>
#include "chess_helper.hpp"
namespace sm
{
    Engine::Engine()
    {
    }
    
    /**
     * @brief the id of the engine
     * 
     * @return const std::string 
     */
    const std::string Engine::getID() const
    {
        return m_id;
    }
    
    /**
     * @brief the author of the engine
     * 
     * @return const std::string 
     */
    const std::string Engine::getAuthor() const
    {
        return m_author;
    }
    
    /**
     * @brief is the engine is currently in debug mode
     * 
     * @return true 
     * @return false 
     */
    bool Engine::inDebugMode() const
    {
        return m_debugMode;
    }
    
    /**
     * @brief set if the engine should be put into debug mode.
     * In debug mode the engine sends a bunch of extra information via the chess-interface
     * 
     * @param debug 
     */
    void Engine::setDebugMode(bool debug)
    {
        m_debugMode = debug;
    }
    
    /**
     * @brief Status if the engine is ready or not
     * 
     * @return true if the engine is ready
     * @return false if the engine is not ready
     */
    bool Engine::isReady() const
    {
        return m_ready;
    }
    
    /**
     * @brief Set the value of a given option to a specific value.
     * This function will throw an exception if the option does not exist
     * 
     * @param option the option to be set
     * @param value the value of the option
     */
    void Engine::setOption(const std::string& option, const std::string& value)
    {
        if(m_engineOptions.isSupported(option)) {
            m_engineOptions.setValue(option, value);
        }
    }

    const EngineOptions& Engine::getOptions() const {
        return m_engineOptions;
    }

    float Engine::max(const Chessposition& pos, int player, int depth, int desiredDepth, float alpha, float beta, int& nodes, Move* out_pMove) const {
        nodes++;

        if(depth == 0) {
            return evaluateBoard(pos.getPosition()) * player;
        }

        auto moves = pos.getValidMoves(true, true);
        if(moves.size() == 0) {
            return evaluateBoard(pos.getPosition()) * player;
        }

        float max = alpha;
        for(const auto& move : moves) {
            Chessposition p = pos;
            p.applyMove(move, false);

            float val = this->min(p, player, depth-1, desiredDepth, max, beta, nodes, out_pMove);
            if(val > max) {
                max = val;
                if(depth == desiredDepth) {
                    *out_pMove = move;
                std::cout << "info score cp " << max * player << " depth " << depth << " seldepth " << desiredDepth << " nodes " << nodes << " time 0 pv " << ChessHelper::moveToString(move) << std::endl;
                }
                if(max >= beta) break;
            }
        }
        return max;
    }

    float Engine::min(const Chessposition& pos, int player, int depth, int desiredDepth, float alpha, float beta, int& nodes, Move* out_pMove) const {
        nodes++;
        
        if(depth == 0) {
            return evaluateBoard(pos.getPosition()) * player;
        }

        auto moves = pos.getValidMoves(true, true);
        if(moves.size() == 0) {
            return evaluateBoard(pos.getPosition()) * player;
        }

        float min = beta;
        for(const auto& move : moves) {
            Chessposition p = pos;
            p.applyMove(move, false);

            float val = this->max(p, player, depth-1, desiredDepth, alpha, min, nodes, out_pMove);
            if(val < min) {
                min = val;
                if(min <= alpha) break;
            }
        }

        return min;
    }

    void Engine::findMove(const Chessposition& pos, Chessposition::Player player, int desiredDepth) const
    {
        sm::Move move;
        int nodes = 0;
        
        int p = (player == Chessposition::Player::WHITE) ? 1 : -1;
        float score = max(m_position, p, desiredDepth, desiredDepth, -99999.f, 99999.f, nodes, &move);

        std::cout << "bestmove " << ChessHelper::moveToString(move) << std::endl;
        spdlog::info("bestmove " + ChessHelper::moveToString(move));
    }

    float Engine::evaluateBoard(const std::array<std::array<char, 8>, 8>& currentBoard) const
    {
        float score = 0.0f;
        // TODO: 
        // King Safety
        // Piece Activity
        // Pawn Structure



        // Variables for Bishop Pair Check
        unsigned short int whiteBishops = 0;
        unsigned short int blackBishops = 0;

        for (int i = 0; i < currentBoard.size(); i++)
        {
            for (int j = 0; j < currentBoard[i].size(); j++)
            {
                switch (currentBoard[i][j])
                {
                case 'p':
                    score += -1.0f;
                    break;
                case 'P':
                    score += 1.0f;
                    break;

                case 'r':
                    score += -5.0f;
                    break;
                case 'R':
                    score += 5.0f;
                    break;

                case 'n':
                    score += -3.25f;
                    break;
                case 'N':
                    score += 3.25f;
                    break;

                case 'q':
                    score += -9.75;
                    break;
                case 'Q':
                    score += 9.75f;
                    break;

                case 'b':
                    score += -3.25f;
                    blackBishops += 1;
                    break;
                case 'B':
                    score += 3.25f;
                    whiteBishops += 1;
                    break;

                default:
                    break;
                }
            }
        }

        if (blackBishops >= 2)
        {
            score += -((int)(blackBishops / 2)) * 0.25f;
        }
        if (whiteBishops >= 2)
        {
            score += ((int)(whiteBishops / 2)) * 0.25f;
        }


        return score;
    }
    
    /**
     * @brief return a reference to the Chessposition object
     * 
     * @return Chessposition& 
     */
    Chessposition& Engine::getPosition()
    {
        return m_position;
    }
    
    /**
     * @brief return a constant reference to the Chessposition object
     * 
     * @return const Chessposition& 
     */
    const Chessposition& Engine::getPosition() const
    {
        return m_position;
    }


    
}