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

        if (depth == 0) {
            return evaluateBoard(pos.getPosition()) * player;
        }

        auto moves = pos.getValidMoves(true, true);
        if (moves.size() == 0) {
            return evaluateBoard(pos.getPosition()) * player;
        }

        float max = alpha;
        for (const auto& move : moves) {
            Chessposition p = pos;
            p.applyMove(move, false);

            float val = this->min(p, player, depth - 1, desiredDepth, max, beta, nodes, out_pMove);
            if (val > max) {
                max = val;
                if (depth == desiredDepth) {
                    *out_pMove = move;
                    std::cout << "info score cp " << max * player << " depth " << depth << " seldepth " << desiredDepth << " nodes " << nodes << " time 0 pv " << ChessHelper::moveToString(move) << std::endl;
                }
                if (max >= beta) break;
            }
        }
        return max;
    }

    float Engine::min(const Chessposition& pos, int player, int depth, int desiredDepth, float alpha, float beta, int& nodes, Move* out_pMove) const {
        nodes++;

        if (depth == 0) {
            return evaluateBoard(pos.getPosition()) * player;
        }

        auto moves = pos.getValidMoves(true, true);
        if (moves.size() == 0) {
            return evaluateBoard(pos.getPosition()) * player;
        }

        float min = beta;
        for (const auto& move : moves) {
            Chessposition p = pos;
            p.applyMove(move, false);

            float val = this->max(p, player, depth - 1, desiredDepth, alpha, min, nodes, out_pMove);
            if (val < min) {
                min = val;
                if (min <= alpha) break;
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
    
    
    bool Engine::hasConnectedPawns(const char color, const unsigned short int p_row, const unsigned short int p_col, Chessposition& currentBoard)
    {
        if (color != 'p' && color != 'P')
        {
            // invalid Input
            return false;
        }

        if (currentBoard.getPosition()[p_row][p_col - 1] == color || currentBoard.getPosition()[p_row][p_col + 1] == color)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool Engine::hasDoublePawns(const char color, const unsigned short int p_row, const unsigned short int p_col, Chessposition& currentBoard)
    {
        if (color != 'p' && color != 'P')
        {
            // invalid Input
            return false;
        }
        
        for (unsigned short i = 0; i < 8; i++)
        {
            if (i == p_row)
            {
                continue;
            }
            if (currentBoard.getPosition()[i][p_col] == color) 
            {
                return true;
            }
        }

        return false;
    }

    bool Engine::hasIsolatedPawns(const char color, const unsigned short int p_col, Chessposition& currentBoard)
    {
        if (color != 'p' && color != 'P')
        {
            // invalid Input
            return false;
        }

        for (unsigned int i = 0; i < 8; i++)
        {
            if (currentBoard.getPosition()[i][p_col - 1] == color || currentBoard.getPosition()[i][p_col + 1] == color)
            {
                return false;
            }
        }

        return true;

    }

    float Engine::evaluateBoard(Chessposition& currentBoard)
    {

        // TODO: 
        // King Safety
        // Piece Activity
        // Pawn Structure

        float score = 0.0f;        

        // Variables for Bishop Pair Check
        unsigned short int whiteBishops = 0;
        unsigned short int blackBishops = 0;

        // Variables for Piece-ACtivity Comparison
        float mod;
        float mod_p = 0.0f;
        float mod_P = 0.0f;
        float mod_r = 0.0f;
        float mod_R = 0.0f;
        float mod_n = 0.0f;
        float mod_N = 0.0f;
        float mod_q = 0.0f;
        float mod_Q = 0.0f;
        float mod_b = 0.0f;
        float mod_B = 0.0f;



        // Material
        for (int i = 0; i < currentBoard.getPosition().size(); i++)
        {
            for (int j = 0; j < currentBoard.getPosition()[i].size(); j++)
            {
                switch (currentBoard.getPosition()[i][j])
                {
                case 'p':
                    mod = (currentBoard.getValidMovesForField(i, j, false, false).size() / AVG_PAWN);
                    mod_p += mod;
                    score += -1.0f * mod;
                    if (hasConnectedPawns('p', i, j, currentBoard))
                    {
                        score -= CONNECTED_PAWNS;
                    }
                    if (hasDoublePawns('p', i, j, currentBoard))
                    {
                        score -= DOUBLE_PAWNS;
                    }
                    if (hasIsolatedPawns('p', j, currentBoard))
                    {
                        score -= ISOLATED_PAWNS;
                    }
                    break;
                case 'P':
                    mod = (currentBoard.getValidMovesForField(i, j, false, false).size() / AVG_PAWN);
                    mod_P += mod;
                    score += 1.0f * mod;
                    if (hasConnectedPawns('P', i, j, currentBoard))
                    {
                        score += CONNECTED_PAWNS;
                    }
                    if (hasDoublePawns('P', i, j, currentBoard))
                    {
                        score += DOUBLE_PAWNS;
                    }
                    if (hasIsolatedPawns('P', j, currentBoard))
                    {
                        score += ISOLATED_PAWNS;
                    }
                    break;

                case 'r':
                    mod = (currentBoard.getValidMovesForField(i, j, false, false).size() / AVG_ROOK);
                    mod_r += mod;
                    score += -5.0f * mod;
                    break;
                case 'R':
                    mod = (currentBoard.getValidMovesForField(i, j, false, false).size() / AVG_ROOK);
                    mod_R += mod;
                    score += 5.0f * mod;
                    break;

                case 'n':
                    mod = (currentBoard.getValidMovesForField(i, j, false, false).size() / AVG_KNIGHT);
                    mod_n += mod;
                    score += -3.25f * mod;
                    break;
                case 'N':
                    mod = (currentBoard.getValidMovesForField(i, j, false, false).size() / AVG_KNIGHT);
                    mod_N += mod;
                    score += 3.25f * mod;
                    break;

                case 'q':
                    mod = (currentBoard.getValidMovesForField(i, j, false, false).size() / AVG_QUEEN);
                    mod_q += mod;
                    score += -9.75 * mod;
                    break;
                case 'Q':
                    mod = (currentBoard.getValidMovesForField(i, j, false, false).size() / AVG_QUEEN);
                    mod_Q += mod;
                    score += 9.75f * mod;
                    break;

                case 'b':
                    mod = (currentBoard.getValidMovesForField(i, j, false, false).size() / AVG_BISHOP);
                    mod_b += mod;
                    score += -3.25f * mod;
                    blackBishops += 1;
                    break;
                case 'B':
                    mod = (currentBoard.getValidMovesForField(i, j, false, false).size() / AVG_BISHOP);
                    mod_B += mod;
                    score += 3.25f * mod;
                    whiteBishops += 1;
                    break;

                default:
                    break;
                }
            }
        }

        // Bishop Pair
        if (blackBishops >= 2)
        {
            score += -((int)(blackBishops / 2)) * 0.25f;
        }
        if (whiteBishops >= 2)
        {
            score += ((int)(whiteBishops / 2)) * 0.25f;
        }

        // Piece-Activity Comparison
        score = score
            - mod_p * PA_IMPACT
            - mod_r * PA_IMPACT
            - mod_n * PA_IMPACT
            - mod_q * PA_IMPACT
            - mod_b * PA_IMPACT
            + mod_P * PA_IMPACT
            + mod_R * PA_IMPACT
            + mod_N * PA_IMPACT
            + mod_Q * PA_IMPACT
            + mod_B * PA_IMPACT;


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