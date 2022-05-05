#include "chessposition.hpp"
#include "chess_helper.hpp"

namespace sm {

    const std::string Chessposition::STARTPOS_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

    void Chessposition::setFEN(std::string p_FEN)
    {
        m_position = sm::ChessHelper::fenToArray(p_FEN);
    }

    void Chessposition::setActivePlayer(int p_id)
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

    int Chessposition::getActivePlayer() const
    {
        return m_activePlayer;
    }

    int Chessposition::getMoveNumber() const
    {
        return m_moveNumber;
    }

    bool Chessposition::isViableMove(const Move& move) const
    {
        const Move& m = move;
        int type = 0;
        char figureChr;
        char figureChrTrgt;

        //Ausfiltern wenn Move au�erhalb des Feldes ist
        if ((m.targetCol > 7 || m.targetCol < 0) || (m.targetRow > 7 || m.targetRow < 0))
        {
            return false;
        }
        //Ausfiltern wenn start Position au�erhalb des Feldes ist
        if ((m.startCol > 7 || m.startCol < 0) || (m.startRow > 7 || m.startRow < 0))
        {
            return false;
        }

        figureChr = m_position[m.startRow][m.startCol];
        figureChrTrgt = m_position[m.targetRow][m.targetCol];

        //�berpr�fen ob an der Prosition eine Figur des aktiven Spielers steht
        switch (getActivePlayer())
        {
            //weiß
        case 0:
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

            //schwarz
        case 1:
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
        }

        if (m.promotion != '\0' && type != 1)
        {
            return false;
        }

        //�berpr�fen, ob der Move f�r die jeweilige Figur g�ltig ist
        int difX = m.targetCol - m.startCol;
        int difY = m.targetRow - m.startRow;
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
            case 0:

                //Bauer geht nur einen nach vorne, bzw 2, wenn es das erste mal ist
                switch (difY)
                {
                case 1:
                    if (figureChrTrgt != '\0')
                        return false;
                    break;
                case 2:
                    if (m.startRow != 1)
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
                    //keine generische Figur auf dem Feld
                    if (figureChrTrgt > 90 || figureChrTrgt == '\0')
                    {
                        //En Passant
                        if (m_previousMove.targetCol == m.targetCol)
                        {
                            if (m_moveCount[m_previousMove.targetRow][m_previousMove.targetCol] == 1)
                            {
                                if (m.targetRow == 5 || m.targetRow == 2)
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

                if (m.promotion != '\0' && m.targetRow != 7)
                    return false;
                break;
            case 1:

                //Bauer geht nur einen nach vorne, bzw 2, wenn es das erste mal ist
                switch (difY)
                {
                case -1:
                    if (figureChrTrgt != '\0')
                        return false;
                    break;
                case -2:
                    if (m.startRow != 6)
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
                    //Pr�fen ob gegnerische figur auf dem feld 
                    if (figureChrTrgt < 97 || figureChrTrgt == '\0')
                    {
                        //En Passant
                        if (m_previousMove.targetCol == m.targetCol)
                        {
                            if (m_moveCount[m_previousMove.targetRow][m_previousMove.targetCol] == 1)
                            {
                                if (m.targetRow == 5 || m.targetRow == 2)
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
                if (m.promotion != '\0' && m.targetRow != 0)
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
                for (int i = m.startCol; i < m.targetCol; i++)
                {
                    //char c = getType(i, m.startRow);
                    char c = m_position[m.startRow][i];
                    if (c != '\0')
                        return false;
                }
            }
            else if (difX < 0)
            {
                for (int i = m.startCol; i > m.targetCol; i--)
                {
                    //char c = getType(i, m.startRow);
                    char c = m_position[m.startRow][i];
                    if (c != '\0')
                        return false;
                }
            }
            if (difY > 0)
            {
                for (int i = m.startRow; i < m.targetRow; i++)
                {
                    //char c = getType(i, m.startRow);
                    char c = m_position[i][m.targetCol];
                    if (c != '\0')
                        return false;
                }
            }
            else if (difY < 0)
            {
                for (int i = m.startRow; i > m.targetRow; i--)
                {
                    //char c = getType(i, m.startRow);
                    char c = m_position[i][m.startCol];
                    if (c != '\0')
                        return false;
                }
            }

            //pr�fen, dass auf dem Zielfeld keine eigene figur steht
            switch (getActivePlayer())
            {
            case 0:
                if (figureChrTrgt != '\0' && figureChrTrgt > 97)
                    return false;
                break;
            case 1:
                if (figureChrTrgt != '\0' && figureChrTrgt < 90)
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
                for (int j = m.startRow; j < m.targetRow; j++)
                {
                    for (int i = m.startCol; i < m.targetCol; i++)
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
                for (int j = m.startRow; j > m.targetRow; j--)
                {
                    for (int i = m.startCol; i > m.targetCol; i--)
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
                for (int j = m.startRow; j > m.targetRow; j--)
                {
                    for (int i = m.startRow; i < m.targetRow; i++)
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
                for (int j = m.startRow; j < m.targetRow; j++)
                {
                    for (int i = m.startCol; i > m.targetCol; i--)
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
            case 0:
                if (figureChrTrgt != '\0' && figureChrTrgt > 97)
                    return false;
                break;
            case 1:
                if (figureChrTrgt != '\0' && figureChrTrgt < 90)
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
                if (m_moveCount[m.startRow][m.startCol] == 0)
                {
                    switch (m.targetCol)
                    {
                    case 1:
                        if (m_moveCount[m.targetRow][m.startCol - 2] == 0)
                            break;
                    case 6:
                        if (m_moveCount[m.targetRow][m.startCol + 1] == 0)
                            break;
                    default:
                        return false;
                    }

                    if (m.startRow != m.targetRow)
                        return false;
                    if (m.startRow != 0 && m.startRow != 7)
                        return false;
                    if (m.startCol != 4)
                        return false;
                    if (m.targetCol != 2 && m.targetCol != 6)
                        return false;


                    //pruefen ob der weg frei ist
                    if (difX > 0)
                    {
                        for (int i = m.startCol; i < m.targetCol; i++)
                        {
                            //char c = getType(i, m.startRow);
                            char c = m_position[m.startRow][i];
                            if (c != '\0')
                                return false;
                        }
                    }
                    else if (difX < 0)
                    {
                        for (int i = m.startCol; i > m.targetCol - 2; i--)
                        {
                            //char c = getType(i, m.startRow);
                            char c = m_position[m.startRow][i];
                            if (c != '\0')
                                return false;
                        }
                    }

                }
            }

            //pr�fen, dass auf dem Zielfeld keine eigene figur steht
            switch (getActivePlayer())
            {
            case 0:
                if (figureChrTrgt != '\0' && figureChrTrgt > 97)
                    return false;
                break;
            case 1:
                if (figureChrTrgt != '\0' && figureChrTrgt < 90)
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
            case 0:
                if (figureChrTrgt != '\0' && figureChrTrgt > 97)
                    return false;
                break;
            case 1:
                if (figureChrTrgt != '\0' && figureChrTrgt < 90)
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
                    for (int j = m.startRow; j < m.targetRow; j++)
                    {
                        for (int i = m.startCol; i < m.targetCol; i++)
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
                    for (int j = m.startRow; j > m.targetRow; j--)
                    {
                        for (int i = m.startCol; i > m.targetCol; i--)
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
                    for (int j = m.startRow; j > m.targetRow; j--)
                    {
                        for (int i = m.startCol; i < m.targetCol; i++)
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
                    for (int j = m.startRow; j < m.targetRow; j++)
                    {
                        for (int i = m.startCol; i > m.targetCol; i--)
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
                case 0:
                    if (figureChrTrgt != '\0' && figureChrTrgt > 97)
                        return false;
                    break;
                case 1:
                    if (figureChrTrgt != '\0' && figureChrTrgt < 90)
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
                    for (int i = m.startCol; i < m.targetCol; i++)
                    {
                        //char c = getType(i, m.startRow);
                        char c = m_position[m.startRow][i];
                        if (c != '\0')
                            return false;
                    }
                }
                else if (difX < 0)
                {
                    for (int i = m.startCol; i > m.targetCol; i--)
                    {
                        //char c = getType(i, m.startRow);
                        char c = m_position[m.startRow][i];
                        if (c != '\0')
                            return false;
                    }
                }
                if (difY > 0)
                {
                    for (int i = m.startRow; i < m.targetRow; i++)
                    {
                        //char c = getType(i, m.startRow);
                        char c = m_position[i][m.startCol];
                        if (c != '\0')
                            return false;
                    }
                }
                else if (difY < 0)
                {
                    for (int i = m.startRow; i > m.targetRow; i--)
                    {
                        //char c = getType(i, m.startRow);
                        char c = m_position[i][m.startCol];
                        if (c != '\0')
                            return false;
                    }
                }

                //pr�fen, dass auf dem Zielfeld keine eigene figur steht
                switch (getActivePlayer())
                {
                case 0:
                    if (figureChrTrgt != '\0' && figureChrTrgt > 97)
                        return false;
                    break;
                case 1:
                    if (figureChrTrgt != '\0' && figureChrTrgt < 90)
                        return false;
                    break;
                }
                break;
            }
            
        }

        return true;
    }

    std::list<Move> Chessposition::getValidMoves(int row, int column) const
    {
        // Comment
        std::list<Move> moves;

        /*for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                Move m;
                m.startRow = row;
                m.startCol = column;
                m.targetCol = j;
                m.targetRow = i;
                for (int c = 0; c < 2; c++)
                {
                    switch (c)
                    {
                    case 0:
                        m.capture = false;
                        break;
                    case 1:
                        m.capture = true;
                    default:
                        break;
                    }

                    for (int p = 0; p < 5; p++)
                    {
                        switch (p)
                        {
                        case 0:
                            m.promotion = '\0';
                            break;
                        case 1:
                            m.promotion = 'Q';
                            break;
                        case 2:
                            m.promotion = 'R';
                            break;
                        case 3:
                            m.promotion = 'B';
                            break;
                        case 4:
                            m.promotion = 'N';
                            break;
                        default:
                            break;
                        }

                        if (isViableMove(m))
                        {
                            moves.push_back(m);
                        }
                    }
                }
            }
        }*/

        auto addViableMoves = [&moves, &row, &column, this](const Move* pMoves, size_t count) -> void{
            Move m;
            for (size_t i = 0; i < count; i++)
            {
                m = pMoves[i];
                m.startRow += row;
                m.targetRow += row;
                m.captureRow += row;
                m.startCol += column;
                m.targetCol += column;
                m.captureCol += column;

                if(this->isViableMove(m)) {
                    moves.push_back(m);
                }
            }
        };

        switch(m_position[row][column]) {
            case 'p':
            case 'P':
                addViableMoves(ChessHelper::PAWN_MOVES.data(), ChessHelper::PAWN_MOVES.size());
                break;
            case 'q':
            case 'Q':
                addViableMoves(ChessHelper::ROOK_MOVES.data(), ChessHelper::ROOK_MOVES.size());
                addViableMoves(ChessHelper::BISHOP_MOVES.data(), ChessHelper::BISHOP_MOVES.size());
                break;
            case 'r':
            case 'R':
                addViableMoves(ChessHelper::ROOK_MOVES.data(), ChessHelper::ROOK_MOVES.size());
                break;
            case 'n':
            case 'N':
                addViableMoves(ChessHelper::KNIGHT_MOVES.data(), ChessHelper::KNIGHT_MOVES.size());
                break;
            case 'b':
            case 'B':
                addViableMoves(ChessHelper::BISHOP_MOVES.data(), ChessHelper::BISHOP_MOVES.size());
                break;
            case 'k':
            case 'K':
                addViableMoves(ChessHelper::KING_MOVES.data(), ChessHelper::KING_MOVES.size());
                break;
            default:
                break;
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

        return true;

    }

    Chessposition::Chessposition()
        : m_position(sm::ChessHelper::fenToArray(Chessposition::STARTPOS_FEN))
    {

    }

    Chessposition::Chessposition(const std::string& fen)
        : m_position(sm::ChessHelper::fenToArray(fen))
    {

    }

    Chessposition::Chessposition(const std::array<std::array<char, 8>, 8>& pos)
        : m_position(pos)
    {

    }

}