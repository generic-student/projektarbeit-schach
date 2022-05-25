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

    const std::array<std::array<int, 8>, 8> &Chessposition::getMoveCount() const
    {
        return m_moveCount;
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
        if (move.promotion != '\0' && (figureChr != 'P' || figureChr != 'p'))
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
        m_MovesSinceCaptureOrPawn++;

        if (m_position[move.startRow][move.startCol] == 'P' || m_position[move.startRow][move.startCol] == 'p')
            m_MovesSinceCaptureOrPawn = 0;

        if (validate && !isViableMove(move))
        {
            return false;
        }

        // CAPTURE the target
        if (move.capture)
        {
            m_position[move.captureRow][move.captureCol] = 0;

            m_MovesSinceCaptureOrPawn = 0;
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
        std::array<std::array<bool, 8>, 8> threat = simulated.generateThreatMap();

        char king = m_activePlayer == Player::WHITE ? 'K' : 'k';
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (threat[i][j] && simulated.getType(i, j) == king)
                    return true;
            }
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

    void Chessposition::clearMoveCount()
    {
        for (auto &row : m_moveCount)
        {
            row.fill(0);
        }
    }

    Chessposition::Chessposition()
        : m_position(sm::ChessHelper::fenToArray(Chessposition::STARTPOS_FEN, nullptr, nullptr, nullptr, nullptr))
    {
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

            // white castling
            if (move.startRow == 0 && move.startCol == 4)
            {
                // small
                if (move.targetRow == 0 && move.targetCol == 6)
                {
                    // check if there is nothing in the way
                    if (m_position[0][move.startCol + 1] != '\0' || m_position[0][move.startCol + 2] != '\0')
                        return false;
                }

                // large
                if (move.targetRow == 0 && move.targetCol == 2)
                {
                    // check if there is nothing in the way
                    if (m_position[0][move.startCol - 1] != '\0' || m_position[0][move.startCol - 2] != '\0' || m_position[0][move.startCol - 3] != '\0')
                        return false;
                }
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

}