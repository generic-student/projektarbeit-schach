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

    void Engine::stop() {
        m_mutex.lock();
        m_stop = true;
        m_mutex.unlock();
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
        if(m_stop) return -9999.f;

        nodes++;

        if (depth == 0) {
            return evaluateBoardSimple(pos) * player;
        }

        auto moves = pos.getValidMoves(true, true);
        if (moves.size() == 0) {
            return evaluateBoardSimple(pos) * player;
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
        if(m_stop) return -9999.f;
        
        nodes++;

        if (depth == 0) {
            return evaluateBoardSimple(pos) * player;
        }

        auto moves = pos.getValidMoves(true, true);
        if (moves.size() == 0) {
            return evaluateBoardSimple(pos) * player;
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

    MinMaxResult Engine::findMove(const Chessposition& pos, Chessposition::Player player, int desiredDepth)
    {
        //set the status of the engine
        m_mutex.lock();
        this->m_ready = false;
        this->m_stop = false;
        m_mutex.unlock();
        
        int p = (player == Chessposition::Player::WHITE) ? 1 : -1;
        MinMaxResult result = {Move::null(), 99999.f * -p, 0, 0};
        float score = max(m_position, p, desiredDepth, desiredDepth, -99999.f, 99999.f, result.nodes, &result.move);

        m_mutex.lock();
        this->m_ready = true;
        this->m_stop = false;
        m_mutex.unlock();

        return result;
    }
    
    
    bool Engine::hasConnectedPawns(const char color, const unsigned short int p_row, const unsigned short int p_col, const Chessposition& currentBoard) const
    {
        if (color != 'p' && color != 'P')
        {
            // invalid Input
            return false;
        }

        if (p_col == 0)
        {
            if (currentBoard.getPosition()[p_row][p_col + 1] == color)
            {
                return true;
            }
        }
        else if (p_col == 7)
        {
            if (currentBoard.getPosition()[p_row][p_col - 1] == color)
            {
                return true;
            }
        }
        else if (currentBoard.getPosition()[p_row][p_col - 1] == color || currentBoard.getPosition()[p_row][p_col + 1] == color)
        {
            return true;
        }
        else
        
        return false;
        
    }

    bool Engine::hasDoublePawns(const char color, const unsigned short int p_row, const unsigned short int p_col, const Chessposition& currentBoard) const
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

    bool Engine::hasIsolatedPawns(const char color, const unsigned short int p_col, const Chessposition& currentBoard) const
    {
        if (color != 'p' && color != 'P')
        {
            // invalid Input
            return false;
        }

        for (unsigned int i = 0; i < 8; i++)
        {
            if (p_col == 0)
            {
                if (currentBoard.getPosition()[i][p_col + 1] == color)
                {
                    return false;
                }
            }
            else if (p_col == 7)
            {
                if (currentBoard.getPosition()[i][p_col - 1] == color)
                {
                    return false;
                }
            }
            else if (currentBoard.getPosition()[i][p_col - 1] == color || currentBoard.getPosition()[i][p_col + 1] == color)
            {
                return false;
            }
        }

        return true;

    }

    float Engine::evaluateBoardSimple(const Chessposition& currentBoard) const {
        float score = 0.0f;
        // TODO: 
        // King Safety
        // Piece Activity
        // Pawn Structure
        // Variables for Bishop Pair Check
        unsigned short int whiteBishops = 0;
        unsigned short int blackBishops = 0;
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                switch (currentBoard.getType(i, j))
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

    float Engine::evaluateBoard(const Chessposition& currentBoard) const
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
                    score += -1.0f *mod;
                    /*if (hasConnectedPawns('p', i, j, currentBoard))
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
                    }*/
                    break;
                case 'P':
                    mod = (currentBoard.getValidMovesForField(i, j, false, false).size() / AVG_PAWN);
                    mod_P += mod;
                    score += 1.0f * mod;
                    /*if (hasConnectedPawns('P', i, j, currentBoard))
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
                    }*/
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

    std::vector<Move> Engine::orderMoves(std::vector<Move> moves)
    {
        return std::vector<Move>();
    }


    
}