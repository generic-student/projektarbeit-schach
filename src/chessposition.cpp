#include "chessposition.hpp"
#include "chess_helper.hpp"
#include <algorithm>

namespace sm
{

    const std::string Chessposition::STARTPOS_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0";

    void Chessposition::setFEN(std::string p_FEN)
    {
        clearMoveCount();
        char player;
        m_position = sm::ChessHelper::fenToArray(p_FEN, &player, nullptr, &m_MovesSinceCaptureOrPawn, &m_moveNumber);
        m_activePlayer = player == 'w' ? Chessposition::Player::WHITE : Chessposition::Player::BLACK;
    }

    void Chessposition::setActivePlayer(Chessposition::Player p_id)
    {
        m_activePlayer = p_id;
    }

    const std::array<std::array<char, 8>, 8> &Chessposition::getPosition() const
    {
        return m_position;
    }

    std::string Chessposition::getFEN() const
    {
        std::string fen = sm::ChessHelper::arrayToFen(m_position);
        fen += m_activePlayer == Player::WHITE ? " w" : " b";
        fen += " - - ";
        fen += std::to_string(m_MovesSinceCaptureOrPawn) + " ";
        fen += std::to_string(m_moveNumber);
        return fen;
    }

    char Chessposition::getType(int row, int column) const
    {
        if(row < 0 || row > 7)
        {
            return '\0';
        }
        if (column < 0 || column > 7)
        {
            return '\0';
        }
        return m_position[row][column];
    }

    int Chessposition::getMoveCountPos(int row, int column) const
    {
        return m_moveCount[row][column];
    }

    Chessposition::Player Chessposition::getActivePlayer() const
    {
        return m_activePlayer;
    }

    int Chessposition::getMoveNumber() const
    {
        return m_moveNumber;
    }

    bool Chessposition::isViableMove(const Move &move, bool checkCaptureTarget, bool checkKingSafety) const
    {
        // Ausfiltern wenn Move außerhalb des Feldes ist
        if ((move.targetCol > 7 || move.targetCol < 0) || (move.targetRow > 7 || move.targetRow < 0))
        {
            return false;
        }
        // Ausfiltern wenn start Position außerhalb des Feldes ist
        if ((move.startCol > 7 || move.startCol < 0) || (move.startRow > 7 || move.startRow < 0))
        {
            return false;
        }

        char figureChr = m_position[move.startRow][move.startCol];
        char figureChrTrgt = m_position[move.targetRow][move.targetCol];

        // Wenn weiß dran ist, darf keine schwarze Figur bewegt werden
        if (m_activePlayer == Player::WHITE && figureChr > 'Z')
        {
            return false;
        }
        // Wenn schwarz dran ist, darf keine weiße Figur bewegt werden
        if (m_activePlayer == Player::BLACK && figureChr < 'a')
        {
            return false;
        }

        //überprüfen, dass es sich um einen Bauern handelt wenn es eine Promotion gibt
        if (move.promotion != '\0' && figureChr != 'P' && figureChr != 'p')
        {
            return false;
        }

        //überprüfen dass das capture target nicht eins der eigenen Figuren ist
        if (move.capture && checkCaptureTarget)
        {
            switch (getActivePlayer())
            {
            case Player::WHITE:
                if (figureChrTrgt < 'a' || figureChrTrgt == '\0')
                    return false;
                break;
            case Player::BLACK:
                if (figureChrTrgt > 'Z' || figureChrTrgt == '\0')
                    return false;
                break;
            }
        }

        //überprüfen, ob der Move für die jeweilige Figur gültig ist
        switch (figureChr)
        {
        case 'P':
        case 'p': // Bauer
            if (!isViableMoveForPawn(move, checkCaptureTarget))
                return false;
            break;
        case 'R':
        case 'r': // Turm
            if (!isViableMoveForRook(move, checkCaptureTarget))
                return false;
            break;
        case 'B': // Läufer
        case 'b': // Läufer
            if (!isViableMoveForBishop(move, checkCaptureTarget))
                return false;
            break;
        case 'K': // König
        case 'k': // König
            if (!isViableMoveForKing(move, checkCaptureTarget))
                return false;
            break;
        case 'N': // Springer
        case 'n': // Springer
            if (!isViableMoveForKnight(move, checkCaptureTarget))
                return false;
            break;
        case 'Q': // Königin
        case 'q': // Königin
            if (!isViableMoveForQueen(move, checkCaptureTarget))
                return false;
            break;
        }

        if (checkKingSafety)
            return isKingAttackableInNextMove(move) == false;
        return true;
    }

    std::vector<Move> Chessposition::getValidMovesForField(int row, int column, bool checkCaptureTarget, bool checkKingSafety) const
    {
        // Comment
        std::vector<Move> moves;
        std::vector<std::pair<const Move *, int>> moveLists;

        switch (m_position[row][column])
        {
        case 'p':
        case 'P':
            moveLists.push_back({ChessHelper::PAWN_MOVES.data(), ChessHelper::PAWN_MOVES.size()});
            break;
        case 'q':
        case 'Q':
            moveLists.push_back({ChessHelper::ROOK_MOVES.data(), ChessHelper::ROOK_MOVES.size()});
            moveLists.push_back({ChessHelper::BISHOP_MOVES.data(), ChessHelper::BISHOP_MOVES.size()});
            break;
        case 'r':
        case 'R':
            moveLists.push_back({ChessHelper::ROOK_MOVES.data(), ChessHelper::ROOK_MOVES.size()});
            break;
        case 'n':
        case 'N':
            moveLists.push_back({ChessHelper::KNIGHT_MOVES.data(), ChessHelper::KNIGHT_MOVES.size()});
            break;
        case 'b':
        case 'B':
            moveLists.push_back({ChessHelper::BISHOP_MOVES.data(), ChessHelper::BISHOP_MOVES.size()});
            break;
        case 'k':
        case 'K':
            moveLists.push_back({ChessHelper::KING_MOVES.data(), ChessHelper::KING_MOVES.size()});
            break;
        default:
            break;
        }

        for (const std::pair<const Move *, int> &movePair : moveLists)
        {
            for (size_t i = 0; i < movePair.second; i++)
            {
                Move m = movePair.first[i];
                m.startRow += row;
                m.targetRow += row;
                m.captureRow += row;
                m.startCol += column;
                m.targetCol += column;
                m.captureCol += column;

                if (isViableMove(m, checkCaptureTarget, checkKingSafety))
                {
                    moves.push_back(m);
                }
            }
        }

        return moves;
    }

    std::vector<Move> Chessposition::getValidMoves(bool checkCaptureTarget, bool checkKingSafety) const
    {
        std::vector<Move> moves;
        for (size_t i = 0; i < 8; i++)
        {
            for (size_t j = 0; j < 8; j++)
            {
                std::vector<Move> movesForField = getValidMovesForField(i, j, checkCaptureTarget, checkKingSafety);
                std::move(movesForField.begin(), movesForField.end(), std::back_inserter(moves));
                // moves.splice(moves.end(), getValidMovesForField(i, j, checkCaptureTarget));
            }
        }
        return moves;
    }

    /**
     * @brief Apply a move to the current board position.
     *
     * @param move: The move that is to be applied.
     * @param validate: If this is set, the move is validated before application.
     *
     * @return Returns false if the move was not valid. Otherwise returns true if the move was applied.
     */
    bool Chessposition::applyMove(const Move &move, bool validate)
    {
       
        if (checkEnding(move))
        {
            return false;
        }

        if (validate && !isViableMove(move))
        {
            return false;
        }

        // CAPTURE the target
        if (move.capture)
        {
            m_position[move.captureRow][move.captureCol] = 0;
        }

        // MOVE the chess piece
        if (move.promotion != '\0')
        {
            // PROMOTION
            m_position[move.targetRow][move.targetCol] = move.promotion;
        }
        else
        {
            m_position[move.targetRow][move.targetCol] = m_position[move.startRow][move.startCol];
        }

        m_position[move.startRow][move.startCol] = '\0';

        //  update moveCount
        m_moveCount[move.targetRow][move.targetCol] = m_moveCount[move.startRow][move.startCol] + 1;
        m_moveCount[move.startRow][move.startCol] = 0;

        m_previousMove = move;

        // update active Player
        m_activePlayer = (m_activePlayer == Player::BLACK) ? Player::WHITE : Player::BLACK;

        return true;
    }

    bool Chessposition::isKingAttackableInNextMove(Move move) const
    {
        Chessposition simulated = *this;

        simulated.applyMove(move, false);

        // simulated.setActivePlayer(m_activePlayer);
        // std::array<std::array<bool, 8>, 8> threat = simulated.generateThreatMap();

        // char king = m_activePlayer == Player::WHITE ? 'K' : 'k';
        // for (int i = 0; i < 8; i++)
        // {
        //     for (int j = 0; j < 8; j++)
        //     {
        //         if (threat[i][j] && simulated.getType(i, j) == king)
        //             return true;
        //     }
        // }

        //big function
        char king = m_activePlayer == Player::WHITE ? 'K' : 'k';
        int king_row, king_col;
        bool king_found = false;
        //get the king
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if(simulated.getType(i, j) == king) {
                    king_row = i;
                    king_col = j;
                    king_found = true;
                    i = 8;
                    j = 8;
                    break;
                }
            }
        }

        if(!king_found) {
            return true;
        }

        char ENEMY_QUEEN = m_activePlayer == Player::WHITE ? 'q' : 'Q';
        char ENEMY_KNIGHT = m_activePlayer == Player::WHITE ? 'n' : 'N';
        char ENEMY_BISHOP = m_activePlayer == Player::WHITE ? 'b' : 'B';
        char ENEMY_ROOK = m_activePlayer == Player::WHITE ? 'r' : 'R';
        char ENEMY_KING = m_activePlayer == Player::WHITE ? 'k' : 'K';

        //left top
        for(int i = 1; i <= std::min(king_row, king_col); i++) {
            char piece = simulated.getType(king_row - i, king_col - i);
            if(piece == ENEMY_QUEEN || piece == ENEMY_BISHOP) {
                return true;
            }
            else if(piece != '\0') {
                break;
            }
        }

        //left bot
        for(int i = 1; i <= std::min(7 - king_row, king_col); i++) {
            char piece = simulated.getType(king_row + i, king_col - i);
            if(piece == ENEMY_QUEEN || piece == ENEMY_BISHOP) {
                return true;
            }
            else if(piece != '\0') {
                break;
            }
        }

        //right top
        for(int i = 1; i <= std::min(king_row, 7 - king_col); i++) {
            char piece = simulated.getType(king_row - i, king_col + i);
            if(piece == ENEMY_QUEEN || piece == ENEMY_BISHOP) {
                return true;
            }
            else if(piece != '\0') {
                break;
            }
        }

        //right bot
        for(int i = 1; i <= std::min(7 - king_row, 7 - king_col); i++) {
            char piece = simulated.getType(king_row + i, king_col + i);
            if(piece == ENEMY_QUEEN || piece == ENEMY_BISHOP) {
                return true;
            }
            else if(piece != '\0') {
                break;
            }
        }

        //top
        for(int i = 1; i <= king_row; i++) {
            char piece = simulated.getType(king_row - i, king_col);
            if(piece == ENEMY_QUEEN || piece == ENEMY_ROOK) {
                return true;
            }
            else if(piece != '\0') {
                break;
            }
        }

        //bot
        for(int i = 1; i <= 7 - king_row; i++) {
            char piece = simulated.getType(king_row + i, king_col);
            if(piece == ENEMY_QUEEN || piece == ENEMY_ROOK) {
                return true;
            }
            else if(piece != '\0') {
                break;
            }
        }

        //left
        for(int i = 1; i <= king_col; i++) {
            char piece = simulated.getType(king_row, king_col - i);
            if(piece == ENEMY_QUEEN || piece == ENEMY_ROOK) {
                return true;
            }
            else if(piece != '\0') {
                break;
            }
        }

        //right
        for(int i = 1; i <= 7 - king_col; i++) {
            char piece = simulated.getType(king_row, king_col + i);
            if(piece == ENEMY_QUEEN || piece == ENEMY_ROOK) {
                return true;
            }
            else if(piece != '\0') {
                break;
            }
        }

        // enemy knights
        if(king_col > 0 && king_row > 1 && simulated.getType(king_row - 2, king_col - 1) == ENEMY_KNIGHT || 
        king_col < 7 && king_row > 1 && simulated.getType(king_row - 2, 7 - king_col) == ENEMY_KNIGHT ||
        king_col < 6 && king_row > 0 && simulated.getType(king_row - 1, 6 - king_col) == ENEMY_KNIGHT ||
        king_col < 6 && king_row < 7 && simulated.getType(7 - king_row, 6 - king_col) == ENEMY_KNIGHT ||
        king_col < 7 && king_row < 6 && simulated.getType(6 - king_row, 7 - king_col) == ENEMY_KNIGHT ||
        king_col > 0 && king_row < 6 && simulated.getType(6 - king_row, king_col - 1) == ENEMY_KNIGHT ||
        king_col > 1 && king_row < 7 && simulated.getType(7 - king_row, king_col - 2) == ENEMY_KNIGHT ||
        king_col > 1 && king_row > 0 && simulated.getType(king_row - 1, king_col - 2) == ENEMY_KNIGHT) {
              return true;
          }
                
        //enemy king
        if(king_col > 0 && king_row > 0 && simulated.getType(king_row - 1, king_col - 1) == ENEMY_KING ||
        king_col < 7 && king_row > 0 && simulated.getType(king_row - 1, king_col + 1) == ENEMY_KING ||
        king_col < 7 && king_row < 7 && simulated.getType(king_row + 1, king_col + 1) == ENEMY_KING ||
        king_col > 0 && king_row < 7 && simulated.getType(king_row + 1, king_col - 1) == ENEMY_KING ||
        king_col > 0 && simulated.getType(king_row, king_col - 1) == ENEMY_KING ||
        king_col < 7 && simulated.getType(king_row, king_col + 1) == ENEMY_KING ||
        king_row > 0 && simulated.getType(king_row - 1, king_col) == ENEMY_KING ||
        king_row < 7 && simulated.getType(king_row + 1, king_col) == ENEMY_KING) {
            return true;
        }

        //enemy pawns
        switch(m_activePlayer) {
            case Player::WHITE:
                if(king_row > 0 && king_col > 0 && simulated.getType(king_row - 1, king_col - 1) == 'p' || 
                king_row > 0 && king_col < 7 && simulated.getType(king_row - 1, king_col + 1) == 'p') {
                    return true;
                }
                break;
            case Player::BLACK:
                if(king_row < 7 && king_col > 0 && simulated.getType(king_row + 1, king_col - 1) == 'P' || 
                king_row < 7 && king_col < 7 && simulated.getType(king_row + 1, king_col + 1) == 'P') {
                    return true;
                }
                break;
        }

        return false;
    }

    std::array<std::array<bool, 8>, 8> Chessposition::generateThreatMap() const
    {
        std::array<std::array<bool, 8>, 8> map;
        for (std::array<bool, 8> &row : map)
        {
            row.fill(false);
        }

        std::vector<Move> viableMoves = getValidMoves(true, false);

        for (auto m : viableMoves)
        {
            int x, y;
            if (m.capture)
            {
                x = m.captureCol;
                y = m.captureRow;
                map[y][x] = true;
            }
        }

        return map;
    }

    bool Chessposition::isPatt() const
    {
        std::vector<Move> viableMoves = getValidMoves();

        if (m_MovesSinceCaptureOrPawn == 50)
            return true;

        if (viableMoves.size() == 0)
            return true;
        return false;
    }

    bool Chessposition::isMatt() const
    {
        std::vector<Move> viableMoves = getValidMoves();

        if (viableMoves.size() == 0)
        {
            std::array<std::array<bool, 8>, 8> threat = generateThreatMap();

            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (threat[i][j])
                    {
                        switch (getActivePlayer())
                        {
                        case Player::WHITE:
                            if (getType(i, j) == 'K')
                                return true;
                            break;
                        case Player::BLACK:
                            if (getType(i, j) == 'k')
                                return true;
                            break;
                        }
                    }
                }
            }
        }

        return false;
    }

    bool Chessposition::checkPositionRepitition()
    {
        std::string currentFEN = getFEN();
        int repititions = 0;
        
        for (auto f : m_lastPositions)
        {
            if (!f.compare(currentFEN))
            {
                repititions++;
            }
        }

        if (repititions == 2)
        {
            return true;
        }
        else
        {
            m_lastPositions.push_back(currentFEN);
        }
        
        return false;
    }

    bool Chessposition::checkMoveCount(const Move& move)
    {
        if (m_MovesSinceCaptureOrPawn == 50)
            return true;

        m_MovesSinceCaptureOrPawn++;

        if (m_position[move.startRow][move.startCol] == 'P' || m_position[move.startRow][move.startCol] == 'p')
            m_MovesSinceCaptureOrPawn = 0;

        if (move.capture)
        {
            m_MovesSinceCaptureOrPawn = 0;
        }

        return false;
    }

    Chessposition::Ending Chessposition::checkEnding(const Move &move)
    {
        if (isPatt())
        {
            return Chessposition::Ending::Draw;
        }

        if (isMatt())
        {
            switch (getActivePlayer())
            {
            case Chessposition::Player::WHITE:
                return Chessposition::Ending::BlackWins;
                break;

            case Chessposition::Player::BLACK:
                return Chessposition::Ending::WhiteWins;
                break;
            }
        }

        if (checkPositionRepitition())
        {
            return Chessposition::Ending::Draw;
        }

        if (checkMoveCount(move))
        {
            return Chessposition::Ending::Draw; 
        }

        return Chessposition::Ending::Ongoing;
    }

    void Chessposition::clearMoveCount()
    {
        for (auto &row : m_moveCount)
        {
            row.fill(0);
        }
    }

    Chessposition::Chessposition()
        //: m_position(sm::ChessHelper::fenToArray(Chessposition::STARTPOS_FEN, nullptr, nullptr, nullptr, nullptr))
    {
        m_position = std::array<std::array<char, 8>, 8>{
            std::array<char, 8>{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
            std::array<char, 8>{'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
            std::array<char, 8>{'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'},
            std::array<char, 8>{'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'},
            std::array<char, 8>{'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'},
            std::array<char, 8>{'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'},
            std::array<char, 8>{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
            std::array<char, 8>{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}};

        clearMoveCount();
        m_previousMove = Move();
    }

    Chessposition::Chessposition(const std::string &fen)
        : m_position()
    {
        setFEN(fen);
        m_previousMove = Move();
    }

    bool Chessposition::isViableMoveForPawn(const Move &move, bool checkCaptureTarget) const
    {
        int difX = move.targetCol - move.startCol;
        int difY = move.targetRow - move.startRow;
        char figureChr = m_position[move.startRow][move.startCol];
        char figureChrTrgt = m_position[move.targetRow][move.targetCol];

        /*
                Bauern Logik

                zuerst pr�fen welcher Spieler an der reihe ist.

                wenn er diagonal geht pr�fen
                Danach pr�fen ob der Bauer gerade oder diagonal geht (x Koordinate �ndert sich),
                Dann �berpr�fen, ob der Bauer genau ein Feld Diagonal geht (x und y Koordinate genau 1 unterschied, f�r y Koordinate je nach Farbe +1 oder -1)
                zuletzt �berpr�fen ob auf dem Feld eine Figur der anderen Farbe steht.

                F�r gerade pr�fen ob es eine oder zwei Felder nach vorne geht (Y-Koordinate 1 oder 2 ge�ndert)

                Wenn es 2 sind pr�fen ob es der erste move dieses Bauern ist

                wenn es einen nach vorne geht pr�fen, ob das Feld frei ist.


                 */

        switch (getActivePlayer())
        {
        case Player::WHITE:

            // Bauer geht nur einen nach vorne, bzw 2, wenn es das erste mal ist
            switch (difY)
            {
            case -1:
                break;
            case -2:
                if (move.startRow != 6)
                    return false;
                if (figureChrTrgt != '\0')
                    return false;
                if (m_position[move.startRow - 1][move.startCol] != '\0')
                    return false;
                break;
            default:
                return false;
            }

            switch (difX)

            {
            case 0:
                if (figureChrTrgt != '\0')
                    return false;
                break;
            case 1:
            case -1:

                // mehr als 1 nach vorne
                if (difY != -1)
                    return false;
                // keine generische Figur auf dem Feld
                if ((figureChrTrgt < 97 || figureChrTrgt == '\0') && checkCaptureTarget)
                {
                    // En Passant
                    if (m_previousMove.targetCol == move.targetCol)
                    {
                        if (m_moveCount[m_previousMove.targetRow][m_previousMove.targetCol] == 1)
                        {
                            if (move.targetRow == 5 || move.targetRow == 2)
                            {
                                if (m_position[m_previousMove.targetRow][m_previousMove.targetCol] == 'P' || m_position[m_previousMove.targetRow][m_previousMove.targetCol] == 'p')
                                {
                                    break;
                                }
                            }
                        }
                    }

                    return false;
                }
                break;
            default:
                return false;
            }

            if (move.promotion != '\0' && move.targetRow != 0)
                return false;

            if (move.targetRow == 0 && move.promotion == '\0')
                return false;

            break;
        case Player::BLACK:

            // Bauer geht nur einen nach vorne, bzw 2, wenn es das erste mal ist
            switch (difY)
            {
            case 1:
                break;
            case 2:
                if (move.startRow != 1)
                    return false;
                if (figureChrTrgt != '\0')
                    return false;
                if (m_position[move.startRow + 1][move.startCol] != '\0')
                    return false;
                break;
            default:
                return false;
            }

            switch (difX)

            {
            case 0:
                if (figureChrTrgt != '\0')
                    return false;
                break;
            case 1:
            case -1:
                // mehr als 1 nach vorne
                if (difY != 1)
                    return false;
                // Pr�fen ob gegnerische figur auf dem feld
                if ((figureChrTrgt > 90 || figureChrTrgt == '\0') && checkCaptureTarget)
                {
                    // En Passant
                    if (m_previousMove.targetCol == move.targetCol)
                    {
                        if (m_moveCount[m_previousMove.targetRow][m_previousMove.targetCol] == 1)
                        {
                            if (move.targetRow == 5 || move.targetRow == 2)
                            {
                                if (m_position[m_previousMove.targetRow][m_previousMove.targetCol] == 'P' || m_position[m_previousMove.targetRow][m_previousMove.targetCol] == 'p')
                                {
                                    break;
                                }
                            }
                        }
                    }

                    return false;
                }
                break;
            default:
                return false;
            }

            if (move.promotion != '\0' && move.targetRow != 7)
                return false;

            if (move.targetRow == 7 && move.promotion == '\0')
                return false;

            break;
        }

        return true;
    }

    bool Chessposition::isViableMoveForBishop(const Move &move, bool checkCaptureTarget) const
    {
        int difX = move.targetCol - move.startCol;
        int difY = move.targetRow - move.startRow;
        char figureChr = m_position[move.startRow][move.startCol];
        char figureChrTrgt = m_position[move.targetRow][move.targetCol];

        /*

                L�ufer Logik

               pr�fen ob der L�ufer diagonal geht (x und y Ver�nderung ist gleich)

               danach pr�fen ob der Weg zum ziel frei ist (Sowohl x und y koordinate immer um 1 dem ziel ann�hern und pr�fen ob das feld frei ist)

               zuletzt pr�fen, ob das ziel Feld Frei ist, oder eine Figur der anderen Farbe drauf ist

                */

        if ((difX * difX) != (difY * difY))
            return false;

        // pr�fen ob weg frei ist diagonal
        // unten rechts
        if (difX > 0 && difY > 0)
        {
            for (int i = 1; i < difY; i++)
            {
                char c = m_position[move.startRow + i][move.startCol + i];
                if (c != '\0')
                    return false;
            }
        }
        // oben links
        else if (difX < 0 && difY < 0)
        {
            for (int i = 1; i < -difY; i++)
            {
                char c = m_position[move.startRow - i][move.startCol - i];
                if (c != '\0')
                    return false;
            }
        }
        // oben rechts
        else if (difX > 0 && difY < 0)
        {
            for (int i = 1; i < -difY; i++)
            {
                char c = m_position[move.startRow - i][move.startCol + i];
                if (c != '\0')
                    return false;
            }
        }
        // unten links
        else if (difX < 0 && difY > 0)
        {
            for (int i = 1; i < difY; i++)
            {
                char c = m_position[move.startRow + i][move.startCol - i];
                if (c != '\0')
                    return false;
            }
        }

        // pr�fen, dass auf dem Zielfeld keine eigene figur steht
        switch (getActivePlayer())
        {
        case Player::WHITE:
            if (figureChrTrgt != '\0' && figureChrTrgt < 90)
                return false;
            break;
        case Player::BLACK:
            if (figureChrTrgt != '\0' && figureChrTrgt > 97)
                return false;
            break;
        }

        return true;
    }

    bool Chessposition::isViableMoveForKnight(const Move &move, bool checkCaptureTarget) const
    {
        int difX = move.targetCol - move.startCol;
        int difY = move.targetRow - move.startRow;
        char figureChr = m_position[move.startRow][move.startCol];
        char figureChrTrgt = m_position[move.targetRow][move.targetCol];

        /*

                Springer Logik

                pr�fen ob der Springer ein Koordinate um 2 und die andere um 1 ver�ndert
                danach pr�fen, ob das Zielfeld leer ist, oder eine Figur der andern Farbe drauf ist.

                */

        // pr�fen, dass auf dem Zielfeld keine eigene figur steht
        switch (getActivePlayer())
        {
        case Player::WHITE:
            if (figureChrTrgt != '\0' && figureChrTrgt < 90)
                return false;
            break;
        case Player::BLACK:
            if (figureChrTrgt != '\0' && figureChrTrgt > 97)
                return false;
            break;
        }

        if (abs(difX) == 2 && abs(difY) == 1)
            return true;
        if (abs(difY) == 2 && abs(difX) == 1)
            return true;

        return false;
    }

    bool Chessposition::isViableMoveForRook(const Move &move, bool checkCaptureTarget) const
    {
        int difX = move.targetCol - move.startCol;
        int difY = move.targetRow - move.startRow;
        char figureChr = m_position[move.startRow][move.startCol];
        char figureChrTrgt = m_position[move.targetRow][move.targetCol];

        /*
               Turm Logik

               pr�fen ob der Turm nur gerade geht (entweder x oder y Koordinate bleibt gleich)

               dann Pr�fen, ob der weg frei ist (Koordinate um 1 erh�hen und pr�fen, ob das Feld frei ist)

               zuletz pr�fen, ob das Ziel feld frei ist, oder eine Figur der anderen Farbe drauf ist
                */

        if (difX != 0 && difY != 0)
            return false;

        // pr�fen ob der weg frei ist gerade
        if (difX > 0)
        {
            for (int i = move.startCol + 1; i < move.targetCol; i++)
            {
                // char c = getType(i, m.startRow);
                char c = m_position[move.startRow][i];
                if (c != '\0')
                    return false;
            }
        }
        else if (difX < 0)
        {
            for (int i = move.startCol - 1; i > move.targetCol; i--)
            {
                // char c = getType(i, m.startRow);
                char c = m_position[move.startRow][i];
                if (c != '\0')
                    return false;
            }
        }
        if (difY > 0)
        {
            for (int i = move.startRow + 1; i < move.targetRow; i++)
            {
                // char c = getType(i, m.startRow);
                char c = m_position[i][move.targetCol];
                if (c != '\0')
                    return false;
            }
        }
        else if (difY < 0)
        {
            for (int i = move.startRow - 1; i > move.targetRow; i--)
            {
                // char c = getType(i, m.startRow);
                char c = m_position[i][move.startCol];
                if (c != '\0')
                    return false;
            }
        }

        // pr�fen, dass auf dem Zielfeld keine eigene figur steht
        switch (getActivePlayer())
        {
        case Player::WHITE:
            if (figureChrTrgt != '\0' && figureChrTrgt < 90)
                return false;
            break;
        case Player::BLACK:
            if (figureChrTrgt != '\0' && figureChrTrgt > 97)
                return false;
            break;
        }

        return true;
    }

    bool Chessposition::isViableMoveForQueen(const Move &move, bool checkCaptureTarget) const
    {
        const bool isViableForRook = isViableMoveForRook(move, checkCaptureTarget);
        const bool isViableForBishop = isViableMoveForBishop(move, checkCaptureTarget);

        /*

               K�nigin Logik

               L�ufer, oder Turm muss passen, also nicht beide falsch

               */

        // the move needs to be valid for the bishop or the rook
        return (isViableForRook || isViableForBishop);

        // //pr�fen ob sie Diagonal oder gerade geht, abh�ngig davon dann Turm obder L�ufer logik w�hlen
        //     //L�ufer
        //     if ((difX * difX) == (difY * difY))
        //     {

        //         //pr�fen ob weg frei ist diagonal
        //         //unten rechts
        //         if (difX > 0 && difY > 0)
        //         {
        //             for (int i = 1; i < difY; i++)
        //             {
        //                 char c = m_position[move.startRow + i][move.startCol + i];
        //                 if (c != '\0')
        //                     return false;
        //             }
        //         }
        //         //oben links
        //         else if (difX < 0 && difY < 0)
        //         {
        //             for (int i = 1; i < -difY; i++)
        //             {
        //                 char c = m_position[move.startRow - i][move.startCol - i];
        //                 if (c != '\0')
        //                     return false;
        //             }
        //         }
        //         //oben rechts
        //         else if (difX > 0 && difY < 0)
        //         {
        //             for (int i = 1; i < -difY; i++)
        //             {
        //                 char c = m_position[move.startRow - i][move.startCol + i];
        //                 if (c != '\0')
        //                     return false;
        //             }
        //         }
        //         //unten links
        //         else if (difX < 0 && difY >0)
        //         {
        //             for (int i = 1; i < difY; i++)
        //             {
        //                 char c = m_position[move.startRow + i][move.startCol - i];
        //                 if (c != '\0')
        //                     return false;
        //             }
        //         }

        //         //pr�fen, dass auf dem Zielfeld keine eigene figur steht
        //         switch (getActivePlayer())
        //         {
        //         case Player::WHITE:
        //             if (figureChrTrgt != '\0' && figureChrTrgt < 90)
        //                 return false;
        //             break;
        //         case Player::BLACK:
        //             if (figureChrTrgt != '\0' && figureChrTrgt > 97)
        //                 return false;
        //             break;
        //         }

        //     }
        //     else if (difX != 0 && difY != 0)
        //     {
        //         return false;
        //     }
        //     //Turm
        //     else
        //     {
        //         //pr�fen ob der weg frei ist gerade
        //         if (difX > 0)
        //         {
        //             for (int i = move.startCol+1; i < move.targetCol; i++)
        //             {
        //                 //char c = getType(i, m.startRow);
        //                 char c = m_position[move.startRow][i];
        //                 if (c != '\0')
        //                     return false;
        //             }
        //         }
        //         else if (difX < 0)
        //         {
        //             for (int i = move.startCol-1; i > move.targetCol; i--)
        //             {
        //                 //char c = getType(i, m.startRow);
        //                 char c = m_position[move.startRow][i];
        //                 if (c != '\0')
        //                     return false;
        //             }
        //         }
        //         if (difY > 0)
        //         {
        //             for (int i = move.startRow+1; i < move.targetRow; i++)
        //             {
        //                 //char c = getType(i, m.startRow);
        //                 char c = m_position[i][move.startCol];
        //                 if (c != '\0')
        //                     return false;
        //             }
        //         }
        //         else if (difY < 0)
        //         {
        //             for (int i = move.startRow-1; i > move.targetRow; i--)
        //             {
        //                 //char c = getType(i, m.startRow);
        //                 char c = m_position[i][move.startCol];
        //                 if (c != '\0')
        //                     return false;
        //             }
        //         }

        //         //pr�fen, dass auf dem Zielfeld keine eigene figur steht
        //         switch (getActivePlayer())
        //         {
        //         case Player::WHITE:
        //             if (figureChrTrgt != '\0' && figureChrTrgt < 90)
        //                 return false;
        //             break;
        //         case Player::BLACK:
        //             if (figureChrTrgt != '\0' && figureChrTrgt > 97)
        //                 return false;
        //             break;
        //         }
        //     }

        // return true;
    }

    bool Chessposition::isViableMoveForKing(const Move &move, bool checkCaptureTarget) const
    {
        int difX = move.targetCol - move.startCol;
        int difY = move.targetRow - move.startRow;
        char figureChr = m_position[move.startRow][move.startCol];
        char figureChrTrgt = m_position[move.targetRow][move.targetCol];
        Chessposition simulated;// = *this;

        /*
               K�nig Logik

               pr�fen ob ob der K�nig maximal ein feld geht (pr�fen ob x und y Ver�nderung <=1 )

                */
        if ((difX * difX) > 1 || (difY * difY) > 1)
        {
            // Castling
            if ((move.startRow == 0 || move.startRow == 7) && move.startCol == 4)
            {
                unsigned short castling_row = move.startRow;
                // small
                if (move.targetRow == castling_row && move.targetCol == 6)
                {
                    // check if there is nothing in the way
                    if (m_position[castling_row][move.startCol + 1] != '\0' ||
                        m_position[castling_row][move.startCol + 2] != '\0')
                        return false;
                }

                // large
                if (move.startRow == castling_row && move.targetCol == 2)
                {
                    // check if there is nothing in the way
                    if (m_position[castling_row][move.startCol - 1] != '\0' ||
                        m_position[castling_row][move.startCol - 2] != '\0' ||
                        m_position[castling_row][move.startCol - 3] != '\0')
                        return false;
                }
            }

            return false;
        }

        // pr�fen, dass auf dem Zielfeld keine eigene figur steht
        switch (getActivePlayer())
        {
        case Player::WHITE:
            if (figureChrTrgt != '\0' && figureChrTrgt < 90)
                return false;
            break;
        case Player::BLACK:
            if (figureChrTrgt != '\0' && figureChrTrgt > 97)
                return false;
            break;
        }

        return true;
    }

}