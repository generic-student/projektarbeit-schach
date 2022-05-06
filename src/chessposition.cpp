#include "chessposition.hpp"
#include "chess_helper.hpp"
#include <algorithm>

namespace sm {

    const std::string Chessposition::STARTPOS_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

    void Chessposition::setFEN(std::string p_FEN)
    {
        m_position = sm::ChessHelper::fenToArray(p_FEN);
    }

    void Chessposition::setActivePlayer(Chessposition::Player p_id)
    {
        m_activePlayer = p_id;
    }

    const std::array<std::array<int, 8>, 8>& Chessposition::getMoveCount() const
    {
        return m_moveCount;
    }

    const std::array<std::array<char, 8>, 8>& Chessposition::getPosition() const
    {
        return m_position;
    }

    std::string Chessposition::getFEN() const
    {
        return sm::ChessHelper::arrayToFen(m_position);
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

    bool Chessposition::isViableMove(const Move& move, bool checkCaptureTarget, bool checkKingSafety) const
    {
        int type = 0;
        char figureChr;
        char figureChrTrgt;

        //Ausfiltern wenn Move au�erhalb des Feldes ist
        if ((move.targetCol > 7 || move.targetCol < 0) || (move.targetRow > 7 || move.targetRow < 0))
        {
            return false;
        }
        //Ausfiltern wenn start Position au�erhalb des Feldes ist
        if ((move.startCol > 7 || move.startCol < 0) || (move.startRow > 7 || move.startRow < 0))
        {
            return false;
        }

        figureChr = m_position[move.startRow][move.startCol];
        figureChrTrgt = m_position[move.targetRow][move.targetCol];

        //�berpr�fen ob an der Prosition eine Figur des aktiven Spielers steht
        switch (getActivePlayer())
        {
            //weiß
        case Player::WHITE:
            switch (figureChr)
            {
            case 'P':
                type = 1;
                break;
            case 'R':
                type = 2;
                break;
            case 'B':
                type = 3;
                break;
            case 'K':
                type = 4;
                break;
            case 'N':
                type = 5;
                break;
            case 'Q':
                type = 6;
                break;
            default:
                return false;
            }
            break;

            //schwarz
        case Player::BLACK:
            switch (figureChr)
            {
            case 'p':
                type = 1;
                break;
            case 'r':
                type = 2;
                break;
            case 'b':
                type = 3;
                break;
            case 'k':
                type = 4;
                break;
            case 'n':
                type = 5;
                break;
            case 'q':
                type = 6;
                break;
            default:
                return false;
            }
            break;
        }

        if (move.promotion != '\0' && type != 1)
        {
            return false;
        }

        if (move.capture && checkCaptureTarget)
        {
            switch (getActivePlayer())
            {
            case Player::WHITE:
                if (figureChrTrgt < 97 || figureChrTrgt == '\0')
                    return false;
                break;
            case Player::BLACK: 
                if (figureChrTrgt > 90 || figureChrTrgt == '\0')
                    return false;
                break;
            }
        }

        //�berpr�fen, ob der Move f�r die jeweilige Figur g�ltig ist
        int difX = move.targetCol - move.startCol;
        int difY = move.targetRow - move.startRow;
        switch (type)
        {
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
        case 1:
            switch (getActivePlayer())
            {
            case Player::WHITE:

                //Bauer geht nur einen nach vorne, bzw 2, wenn es das erste mal ist
                switch (difY)
                {
                case -1:
                    if (figureChrTrgt != '\0')
                        return false;
                    break;
                case -2:
                    if (move.startRow != 6)
                        return false;
                    if (figureChrTrgt != '\0')
                        return false;
                    break;
                default:
                    return false;
                }

                switch (difX)

                {
                case 0:
                    break;
                case 1:
                case -1:
                    //mehr als 1 nach vorne
                    if (difY != -1)
                        return false;
                    //keine generische Figur auf dem Feld
                    if ((figureChrTrgt < 97 || figureChrTrgt == '\0')&&checkCaptureTarget)
                    {
                        //En Passant
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
                break;
            case Player::BLACK:

                //Bauer geht nur einen nach vorne, bzw 2, wenn es das erste mal ist
                switch (difY)
                {
                case 1:
                    if (figureChrTrgt != '\0')
                        return false;
                    break;
                case 2:
                    if (move.startRow != 1)
                        return false;
                    if (figureChrTrgt != '\0')
                        return false;
                    break;
                default:
                    return false;
                }

                switch (difX)

                {
                case 0:
                    break;
                case 1:
                case -1:
                    //mehr als 1 nach vorne
                    if (difY != 1)
                        return false;
                    //Pr�fen ob gegnerische figur auf dem feld 
                    if ((figureChrTrgt > 90 || figureChrTrgt == '\0')&& checkCaptureTarget)
                    {
                        //En Passant
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
                break;
            }
            break;

            /*
                Turm Logik

                pr�fen ob der Turm nur gerade geht (entweder x oder y Koordinate bleibt gleich)

                dann Pr�fen, ob der weg frei ist (Koordinate um 1 erh�hen und pr�fen, ob das Feld frei ist)

                zuletz pr�fen, ob das Ziel feld frei ist, oder eine Figur der anderen Farbe drauf ist
                 */
        case 2:
            if (difX != 0 && difY != 0)
                return false;

            //pr�fen ob der weg frei ist gerade
            if (difX > 0)
            {
                for (int i = move.startCol; i < move.targetCol; i++)
                {
                    //char c = getType(i, m.startRow);
                    char c = m_position[move.startRow][i];
                    if (c != '\0')
                        return false;
                }
            }
            else if (difX < 0)
            {
                for (int i = move.startCol; i > move.targetCol; i--)
                {
                    //char c = getType(i, m.startRow);
                    char c = m_position[move.startRow][i];
                    if (c != '\0')
                        return false;
                }
            }
            if (difY > 0)
            {
                for (int i = move.startRow; i < move.targetRow; i++)
                {
                    //char c = getType(i, m.startRow);
                    char c = m_position[i][move.targetCol];
                    if (c != '\0')
                        return false;
                }
            }
            else if (difY < 0)
            {
                for (int i = move.startRow; i > move.targetRow; i--)
                {
                    //char c = getType(i, m.startRow);
                    char c = m_position[i][move.startCol];
                    if (c != '\0')
                        return false;
                }
            }

            //pr�fen, dass auf dem Zielfeld keine eigene figur steht
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
            break;


            /*

                L�ufer Logik

               pr�fen ob der L�ufer diagonal geht (x und y Ver�nderung ist gleich)

               danach pr�fen ob der Weg zum ziel frei ist (Sowohl x und y koordinate immer um 1 dem ziel ann�hern und pr�fen ob das feld frei ist)

               zuletzt pr�fen, ob das ziel Feld Frei ist, oder eine Figur der anderen Farbe drauf ist

                */
        case 3:

            if ((difX * difX) != (difY * difY))
                return false;

            //pr�fen ob weg frei ist diagonal
            //unten rechts
            if (difX > 0 && difY > 0)
            {
                for (int j = move.startRow; j < move.targetRow; j++)
                {
                    for (int i = move.startCol; i < move.targetCol; i++)
                    {
                        //char c = getType(i, j);
                        char c = m_position[j][i];
                        if (c != '\0')
                            return false;
                    }
                }
            }
            //oben links
            if (difX < 0 && difY < 0)
            {
                for (int j = move.startRow; j > move.targetRow; j--)
                {
                    for (int i = move.startCol; i > move.targetCol; i--)
                    {
                        //char c = getType(i, j);
                        char c = m_position[j][i];
                        if (c != '\0')
                            return false;
                    }
                }
            }
            //oben rechts
            else if (difX > 0 && difY < 0)
            {
                for (int j = move.startRow; j > move.targetRow; j--)
                {
                    for (int i = move.startCol; i < move.targetCol; i++)
                    {
                        //char c = getType(i, j);
                        char c = m_position[j][i];
                        if (c != '\0')
                            return false;
                    }
                }
            }
            //unten links
            else if (difX < 0 && difY >0)
            {
                for (int j = move.startRow; j < move.targetRow; j++)
                {
                    for (int i = move.startCol; i > move.targetCol; i--)
                    {
                        //char c = getType(i, j);
                        char c = m_position[j][i];
                        if (c != '\0')
                            return false;
                    }
                }
            }


            //pr�fen, dass auf dem Zielfeld keine eigene figur steht
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
            break;


            /*
                K�nig Logik

                pr�fen ob ob der K�nig maximal ein feld geht (pr�fen ob x und y Ver�nderung <=1 )

                 */
        case 4:

            if ((difX * difX) > 1 || (difY * difY) > 1)
            {
                //Rocharde
                //pruefen ob Koenig und Turm sich noch nicht bewegt haben
                if (m_moveCount[move.startRow][move.startCol] == 0)
                {
                    switch (move.targetCol)
                    {
                    case 1:
                        if (m_moveCount[move.targetRow][move.startCol - 2] == 0)
                            break;
                    case 6:
                        if (m_moveCount[move.targetRow][move.startCol + 1] == 0)
                            break;
                    default:
                        return false;
                    }

                    if (move.startRow != move.targetRow)
                        return false;
                    if (move.startRow != 0 && move.startRow != 7)
                        return false;
                    if (move.startCol != 4)
                        return false;
                    if (move.targetCol != 2 && move.targetCol != 6)
                        return false;


                    //pruefen ob der weg frei ist
                    if (difX > 0)
                    {
                        for (int i = move.startCol; i < move.targetCol; i++)
                        {
                            //char c = getType(i, m.startRow);
                            char c = m_position[move.startRow][i];
                            if (c != '\0')
                                return false;
                        }
                    }
                    else if (difX < 0)
                    {
                        for (int i = move.startCol; i > move.targetCol - 2; i--)
                        {
                            //char c = getType(i, m.startRow);
                            char c = m_position[move.startRow][i];
                            if (c != '\0')
                                return false;
                        }
                    }

                }
            }

            //pr�fen, dass auf dem Zielfeld keine eigene figur steht
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
            break;


            /*

                Springer Logik

                pr�fen ob der Springer ein Koordinate um 2 und die andere um 1 ver�ndert
                danach pr�fen, ob das Zielfeld leer ist, oder eine Figur der andern Farbe drauf ist.

                */
        case 5:

            //pr�fen, dass auf dem Zielfeld keine eigene figur steht
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
                break;
            if (abs(difY) == 2 && abs(difX) == 1)
                break;
           

            return false;
            break;


            /*

                K�nigin Logik

                L�ufer, oder Turm muss passen, also nicht beide falsch

                */
        case 6:

            //pr�fen ob sie Diagonal oder gerade geht, abh�ngig davon dann Turm obder L�ufer logik w�hlen
            //L�ufer
            if ((difX * difX) == (difY * difY))
            {

                //pr�fen ob weg frei ist diagonal
                //unten rechts
                if (difX > 0 && difY > 0)
                {
                    for (int j = move.startRow; j < move.targetRow; j++)
                    {
                        for (int i = move.startCol; i < move.targetCol; i++)
                        {
                            //char c = getType(i, j);
                            char c = m_position[j][i];
                            if (c != '\0')
                                return false;
                        }
                    }
                }
                //oben links
                else if (difX < 0 && difY < 0)
                {
                    for (int j = move.startRow; j > move.targetRow; j--)
                    {
                        for (int i = move.startCol; i > move.targetCol; i--)
                        {
                            //char c = getType(i, j);
                            char c = m_position[j][i];
                            if (c != '\0')
                                return false;
                        }
                    }
                }
                //oben rechts
                else if (difX > 0 && difY < 0)
                {
                    for (int j = move.startRow; j > move.targetRow; j--)
                    {
                        for (int i = move.startCol; i < move.targetCol; i++)
                        {
                            //char c = getType(i, j);
                            char c = m_position[j][i];
                            if (c != '\0')
                                return false;
                        }
                    }
                }
                //unten links
                else if (difX < 0 && difY >0)
                {
                    for (int j = move.startRow; j < move.targetRow; j++)
                    {
                        for (int i = move.startCol; i > move.targetCol; i--)
                        {
                            //char c = getType(i, j);
                            char c = m_position[j][i];
                            if (c != '\0')
                                return false;
                        }
                    }
                }


                //pr�fen, dass auf dem Zielfeld keine eigene figur steht
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
                break;

            }
            else if (difX != 0 && difY != 0)
            {
                return false;
            }
            //Turm
            else
            {
                //pr�fen ob der weg frei ist gerade
                if (difX > 0)
                {
                    for (int i = move.startCol; i < move.targetCol; i++)
                    {
                        //char c = getType(i, m.startRow);
                        char c = m_position[move.startRow][i];
                        if (c != '\0')
                            return false;
                    }
                }
                else if (difX < 0)
                {
                    for (int i = move.startCol; i > move.targetCol; i--)
                    {
                        //char c = getType(i, m.startRow);
                        char c = m_position[move.startRow][i];
                        if (c != '\0')
                            return false;
                    }
                }
                if (difY > 0)
                {
                    for (int i = move.startRow; i < move.targetRow; i++)
                    {
                        //char c = getType(i, m.startRow);
                        char c = m_position[i][move.startCol];
                        if (c != '\0')
                            return false;
                    }
                }
                else if (difY < 0)
                {
                    for (int i = move.startRow; i > move.targetRow; i--)
                    {
                        //char c = getType(i, m.startRow);
                        char c = m_position[i][move.startCol];
                        if (c != '\0')
                            return false;
                    }
                }

                //pr�fen, dass auf dem Zielfeld keine eigene figur steht
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
                break;
            }
            
        }

        return (!checkKingSafety || !isKingAttackableInNextMove(move));
    }

    std::vector<Move> Chessposition::getValidMovesForField(int row, int column, bool checkCaptureTarget, bool checkKingSafety) const
    {
        // Comment
        std::vector<Move> moves;
        std::vector<std::pair<const Move*, int>> moveLists;

        switch(m_position[row][column]) {
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

        for(const std::pair<const Move*, int>& movePair : moveLists) {
            for (size_t i = 0; i < movePair.second; i++)
            {
                Move m = movePair.first[i];
                m.startRow += row;
                m.targetRow += row;
                m.captureRow += row;
                m.startCol += column;
                m.targetCol += column;
                m.captureCol += column;

                if(isViableMove(m,checkCaptureTarget,checkKingSafety)) {
                    moves.push_back(m);
                }
            }
            
        }

        return moves;
    }

    std::vector<Move> Chessposition::getValidMoves(bool checkCaptureTarget, bool checkKingSafety) const {
        std::vector<Move> moves;
        for (size_t i = 0; i < 8; i++)
        {
            for (size_t j = 0; j < 8; j++)
            {
                std::vector<Move> movesForField = getValidMovesForField(i, j, checkCaptureTarget, checkKingSafety);
                std::move(movesForField.begin(), movesForField.end(), std::back_inserter(moves));
                //moves.splice(moves.end(), getValidMovesForField(i, j, checkCaptureTarget));
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
    bool Chessposition::applyMove(const Move& move, bool validate)
    {
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

        //update active Player
        switch (getActivePlayer())
        {
        case Player::WHITE:
                setActivePlayer(Player::BLACK);
                break;
        case Player::BLACK: 
                setActivePlayer(Player::WHITE);
                break;
        }

        return true;

    }

    bool Chessposition::isKingAttackableInNextMove(Move move) const
    {
        Chessposition simulated;// = *this;

        simulated.applyMove(move, false);

        std::array<std::array<bool, 8>, 8> threat = simulated.generateThreatMap();

        for (int i = 0; i<8;i++)
        {
            for (int j = 0; j<8; j++)
            {
                if (threat[i][j])
                {
                    switch (getActivePlayer())
                    {
                    case Player::WHITE:
                        if (simulated.getType(i, j) == 'K')
                            return true;
                        break;
                    case Player::BLACK:
                        if (simulated.getType(i, j) == 'k')
                            return true;
                        break;
                    }
                    
                }
            }
        }
        
        return false;
    }

    std::array<std::array<bool, 8>, 8> Chessposition::generateThreatMap()
    {
        std::array<std::array<bool, 8>, 8> map;
        for (std::array<bool, 8>& row : map)
        {
            row.fill(false);
        }

        std::vector<Move> viableMoves = getValidMoves(false, false);

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

    bool Chessposition::isPatt()
    {

        std::vector<Move> viableMoves = getValidMoves();

        if (viableMoves.size() == 0 )
            return true;
        return false;
    }

    Chessposition::Chessposition()
        : m_position(sm::ChessHelper::fenToArray(Chessposition::STARTPOS_FEN))
    {
        m_moveCount = std::array<std::array<int, 8>, 8>();
        m_previousMove = Move();
    }

    Chessposition::Chessposition(const std::string& fen)
        : m_position(sm::ChessHelper::fenToArray(fen))
    {
        m_moveCount = std::array<std::array<int, 8>, 8>();
        m_previousMove = Move();
    }

    Chessposition::Chessposition(const std::array<std::array<char, 8>, 8>& pos)
        : m_position(pos)
    {
        m_moveCount = std::array<std::array<int, 8>, 8>();
        m_previousMove = Move();
    }

    // Chessposition::Chessposition(const Chessposition& cp) {

    // }

}