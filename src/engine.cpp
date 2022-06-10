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
            return evaluateBoard(pos) * player;
        }

        auto moves = pos.getValidMoves(true, true);

        orderMoves(moves);

        if (moves.size() == 0) {
            if(pos.isMatt()) {
                return -9999.f;
            }
            if(pos.isPatt()) {
                return 0.f;
            }
            return evaluateBoard(pos) * player;
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
            return evaluateBoard(pos) * player;
        }

        auto moves = pos.getValidMoves(true, true);

        orderMoves(moves);

        if (moves.size() == 0) {
            if(pos.isMatt()) {
                return 9999.f;
            }
            if(pos.isPatt()) {
                return 0.f;
            }
            return evaluateBoard(pos) * player;
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
    
    
    bool Engine::isConnectedPawn(const char color, const unsigned short int p_row, const unsigned short int p_col, const Chessposition& currentBoard) const
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
        
        return false;
        
    }

    bool Engine::isDoublePawn(const char color, const unsigned short int p_row, const unsigned short int p_col, const Chessposition& currentBoard) const
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

    bool Engine::isIsolatedPawn(const char color, const unsigned short int p_col, const Chessposition& currentBoard) const
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

    bool Engine::isBackwardsPawn(const char color, const unsigned short int p_row, const unsigned short int p_col, const Chessposition& currentBoard) const
    {
        if (color != 'p' && color != 'P')
        {
            // invalid Input
            return false;
        }

        if (color == 'p')
        {
            if (p_row > 5)
            {
                return false;
            }
            if (p_col == 0)
            {
                if (!isConnectedPawn(color, p_row, p_col, currentBoard) && currentBoard.getPosition()[p_row + 2][p_col + 1] == 'P')
                {
                    return true;
                }
            }
            else if (p_col == 7)
            {
                if (!isConnectedPawn(color, p_row, p_col, currentBoard) && currentBoard.getPosition()[p_row + 2][p_col - 1] == 'P')
                {
                    return true;
                }
            }
            else
            {
                if (!isConnectedPawn(color, p_row, p_col, currentBoard) && (currentBoard.getPosition()[p_row + 2][p_col - 1] == 'P' || currentBoard.getPosition()[p_row + 2][p_col + 1] == 'P'))
                {
                    return true;
                }
            }
        }
        else if (color == 'p')
        {
            if (p_row < 2)
            {
                return false;
            }
            if (p_col == 0)
            {
                if (!isConnectedPawn(color, p_row, p_col, currentBoard) && currentBoard.getPosition()[p_row - 2][p_col + 1] == 'P')
                {
                    return true;
                }
            }
            else if (p_col == 7)
            {
                if (!isConnectedPawn(color, p_row, p_col, currentBoard) && currentBoard.getPosition()[p_row - 2][p_col - 1] == 'P')
                {
                    return true;
                }
            }
            else
            {
                if (!isConnectedPawn(color, p_row, p_col, currentBoard) && (currentBoard.getPosition()[p_row - 2][p_col - 1] == 'P' || currentBoard.getPosition()[p_row - 2][p_col + 1] == 'P'))
                {
                    return true;
                }
            }
        }
        return false;
    }
        
    
    bool Engine::isPawnChain(const char color, const unsigned short int p_row, const unsigned short int p_col, const Chessposition& currentBoard) const
    {
        if (color != 'p' && color != 'P')
        {
            // invalid Input
            return false;
        }
        if (p_row == 0 || p_row == 7)
        {
            // ist sicher keine Pawn Chain
            return false;
        }

        if (p_col == 0)
        {
            if (currentBoard.getPosition()[p_row - 1][p_col + 1] == color || currentBoard.getPosition()[p_row + 1][p_col + 1] == color)
            {
                return true;
            }
        }
        else if (p_col == 7)
        {
            if (currentBoard.getPosition()[p_row - 1][p_col - 1] == color || currentBoard.getPosition()[p_row + 1][p_col - 1] == color)
            {
                return true;
            }
        }
        else
        {
            if (currentBoard.getPosition()[p_row - 1][p_col - 1] == color || currentBoard.getPosition()[p_row + 1][p_col - 1] == color || currentBoard.getPosition()[p_row - 1][p_col + 1] == color || currentBoard.getPosition()[p_row + 1][p_col + 1] == color)
            {
                return true;
            }
        }
        return false;
    }

    bool Engine::isPassedPawn(const char color, const unsigned short int p_row, const unsigned short int p_col, const Chessposition& currentBoard) const
    {
        if (color != 'p' && color != 'P')
        {
            // invalid Input
            return false;
        }

        if (color == 'p')
        {
            if (p_col == 0)
            {
                for (unsigned short int i = p_row; i < 8; i++)
                {
                    if (currentBoard.getPosition()[i][p_col] == 'P' || currentBoard.getPosition()[i][p_col + 1] == 'P')
                    {
                        return false;
                    }
                }
                return true;
            }
            else if (p_col == 7)
            {
                for (unsigned short int i = p_row; i < 8; i++)
                {
                    if (currentBoard.getPosition()[i][p_col - 1] == 'P' || currentBoard.getPosition()[i][p_col] == 'P')
                    {
                        return false;
                    }
                }
                return true;
            }
            else
            {
                for (unsigned short int i = p_row; i < 8; i++)
                {
                    if (currentBoard.getPosition()[i][p_col - 1] == 'P' || currentBoard.getPosition()[i][p_col] == 'P' || currentBoard.getPosition()[i][p_col + 1] == 'P')
                    {
                        return false;
                    }
                }
                return true;
            }
            
        }
        else if (color == 'P')
        {
            if (p_col == 0)
            {
                for (unsigned short int i = p_row; i < 8; i--)
                {
                    if (currentBoard.getPosition()[i][p_col] == 'p' || currentBoard.getPosition()[i][p_col + 1] == 'p')
                    {
                        return false;
                    }
                }
                return true;
            }
            else if (p_col == 7)
            {
                for (unsigned short int i = p_row; i < 8; i--)
                {
                    if (currentBoard.getPosition()[i][p_col - 1] == 'p' || currentBoard.getPosition()[i][p_col] == 'p')
                    {
                        return false;
                    }
                }
                return true;
            }
            else
            {
                for (unsigned short int i = p_row; i < 8; i--)
                {
                    if (currentBoard.getPosition()[i][p_col - 1] == 'p' || currentBoard.getPosition()[i][p_col] == 'p' || currentBoard.getPosition()[i][p_col + 1] == 'p')
                    {
                        return false;
                    }
                }
                return true;
            }
        }
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
                    score += -PAWN *mod - j * PROMOTION_RANGE_MOD;
                    if (isConnectedPawn('p', i, j, currentBoard))
                    {
                        score -= CONNECTED_PAWNS;
                    }
                    if (isDoublePawn('p', i, j, currentBoard))
                    {
                        score -= DOUBLE_PAWNS;
                    }
                    if (isIsolatedPawn('p', j, currentBoard))
                    {
                        score -= ISOLATED_PAWNS;
                    }
                    break;
                case 'P':
                    mod = (currentBoard.getValidMovesForField(i, j, false, false).size() / AVG_PAWN);
                    mod_P += mod;
                    score += PAWN * mod + (7 - j) * PROMOTION_RANGE_MOD;
                    if (isConnectedPawn('P', i, j, currentBoard))
                    {
                        score += CONNECTED_PAWNS;
                    }
                    if (isDoublePawn('P', i, j, currentBoard))
                    {
                        score += DOUBLE_PAWNS;
                    }
                    if (isIsolatedPawn('P', j, currentBoard))
                    {
                        score += ISOLATED_PAWNS;
                    }
                    break;

                case 'r':
                    mod = (currentBoard.getValidMovesForField(i, j, false, false).size() / AVG_ROOK);
                    mod_r += mod;
                    score += -ROOK * mod;
                    break;
                case 'R':
                    mod = (currentBoard.getValidMovesForField(i, j, false, false).size() / AVG_ROOK);
                    mod_R += mod;
                    score += ROOK * mod;
                    break;

                case 'n':
                    mod = (currentBoard.getValidMovesForField(i, j, false, false).size() / AVG_KNIGHT);
                    mod_n += mod;
                    score += -KNIGHT * mod;
                    if (i == 0 || i == 7 || j == 0 || j == 7)
                    {
                        score += KNIGHT_EDGE;
                    }
                    break;
                case 'N':
                    mod = (currentBoard.getValidMovesForField(i, j, false, false).size() / AVG_KNIGHT);
                    mod_N += mod;
                    score += KNIGHT * mod;
                    if (i == 0 || i == 7 || j == 0 || j == 7)
                    {
                        score -= KNIGHT_EDGE;
                    }
                    break;

                case 'q':
                    mod = (currentBoard.getValidMovesForField(i, j, false, false).size() / AVG_QUEEN);
                    mod_q += mod;
                    score += -QUEEN * mod;
                    break;
                case 'Q':
                    mod = (currentBoard.getValidMovesForField(i, j, false, false).size() / AVG_QUEEN);
                    mod_Q += mod;
                    score += QUEEN * mod;
                    break;

                case 'b':
                    mod = (currentBoard.getValidMovesForField(i, j, false, false).size() / AVG_BISHOP);
                    mod_b += mod;
                    score += -BISHOP * mod;
                    blackBishops += 1;
                    break;
                case 'B':
                    mod = (currentBoard.getValidMovesForField(i, j, false, false).size() / AVG_BISHOP);
                    mod_B += mod;
                    score += BISHOP * mod;
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
            score += -((int)(blackBishops / 2)) * BISHOP_PAIR_MOD;
        }
        if (whiteBishops >= 2)
        {
            score += ((int)(whiteBishops / 2)) * BISHOP_PAIR_MOD;
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


    /**
     * @brief orders the given List of moves by a vague Evaluation 
     *
     * @return std::vector<Move>
     */
    void Engine::orderMoves(std::vector<Move>& moves) const
    {
        float moveEval[218];
        float captureMultiplier = 3;
        float pawnControl = 3;

        moveEval[0] = 0;

        for (int i = 0; i< moves.size(); i++)
        {
            Move m = moves[i];

            float score = 0;
            char movedPiece = m_position.getType(m.startRow, m.startCol);
            char capturePiece = m_position.getType(m.captureRow, m.captureCol);
            float capturePieceValue = 0;
            float pieceValue = 0;

            switch (capturePiece)
            {
            case 'p':
            case 'P':
                capturePieceValue = PAWN;
                break;
            case 'r':
            case 'R':
                capturePieceValue = ROOK;
                break;
            case 'n':
            case 'N':
                capturePieceValue = KNIGHT;
                break;
            case 'q':
            case 'Q':
                capturePieceValue = QUEEN;
                break;
            case 'b':
            case 'B':
                capturePieceValue = BISHOP;
                break;
            }
            switch (movedPiece)
            {
            case 'p':
            case 'P':
                pieceValue = PAWN;
                break;
            case 'r':
            case 'R':
                pieceValue = ROOK;
                break;
            case 'n':
            case 'N':
                pieceValue = KNIGHT;
                break;
            case 'q':
            case 'Q':
                pieceValue = QUEEN;
                break;
            case 'b':
            case 'B':
                pieceValue = BISHOP;
                break;
            }

            //Wenn eine gegnerische Figur geschlagen wird, soll die m�glichst beste Figur des Gegners,
            //mit der m�glichst schlechtesten eigenen Figur zuerst �berpr�ft werden
            if (m.capture)
            {
                score += capturePieceValue * captureMultiplier - pieceValue;
            }

            //Wenn ein Bauer promoted wird soll zuerst die K�nigin und dann die andern Figuren bewertet werden
            //(wird vermutlich immer die K�nigin werden, da die Evaluation die Position der einzelnen
            // Figuren nicht bewertet und somit die K�nigin immer am besten ist)
            if (m.promotion != '\0')
            {
                switch (m.promotion)
                {
                case 'r':
                case 'R':
                    score += ROOK;
                    break;
                case 'n':
                case 'N':
                    score += KNIGHT;
                    break;
                case 'q':
                case 'Q':
                    score += QUEEN;
                    break;
                case 'b':
                case 'B':
                    score += BISHOP;
                    break;
                default:
                    break;
                }
            }

            //Wenn kein Bauer bewegt wird und dies höherwertige Figur von einem gegnerischen Bauer geschlagen werden kann,
            //ist dies eher schlecht und der move sollte erst später bewertet werden
            if (movedPiece != 'p' && movedPiece != 'P')
            {
                if (isAttackableByPawn(m.targetRow,m.targetCol))
                {
                    score -= pawnControl;
                }
            }

            moveEval[i] = score;
        }

        //Z�ge anhand der groben Evaluierung sortieren
        for (int i = 0; i < (int)moves.size() - 1; i++) {
            for (int j = i + 1; j > 0; j--) {
                int swap = j - 1;
                if (moveEval[swap] < moveEval[j]) {
                    
                    Move tempMove = moves[j];
                    float tempEval = moveEval[j];

                    moves[j] = moves[swap];
                    moves[swap] = tempMove;

                    moveEval[j] = moveEval[swap];
                    moveEval[swap] = tempEval;
                }
            }
        }
    }


    /**
     * @brief checks if the Position is atacked by an enemy Pawn
     *
     * @return bool
     */
    bool Engine::isAttackableByPawn(int row, int col) const
    {
        switch (m_position.getActivePlayer())
        {
        case(Chessposition::Player::WHITE):
            if (m_position.getType(row - 1, col - 1) == 'p' || m_position.getType(row - 1, col + 1) == 'p')
            {
                return true;
            }
            break;
        case(Chessposition::Player::BLACK):
            if (m_position.getType(row + 1, col + 1) == 'P' || m_position.getType(row + 1, col - 1) == 'P')
            {
                return true;
            }
            break;
        default:
            break;
        }
        
        return false;
    }
    
}