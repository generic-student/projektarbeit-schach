#include "chessposition.hpp"
#include "chess_helper.hpp"

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

char Chessposition::getType(int p_x, int p_y) const
{
    return m_position[p_x][p_y];
}

int Chessposition::getMoveCountPos(int p_x, int p_y) const
{
    return m_moveCount[p_x][p_y];
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
    if ((m.targetX > 7 || m.targetX < 0) || (m.targetY > 7 || m.targetY < 0))
    {
        return false;
    }
    //Ausfiltern wenn start Position au�erhalb des Feldes ist
    if ((m.startX > 7 || m.startX < 0) || (m.startY > 7 || m.startY < 0))
    {
        return false;
    }

    figureChr = getType(m.startX, m.startY);
    figureChrTrgt = getType(m.targetX, m.targetY);

    //�berpr�fen ob an der Prosition eine Figur des aktiven Spielers steht
    switch (getActivePlayer())
    {
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

    //�berpr�fen, ob der Move f�r die jeweilige Figur g�ltig ist

    int difX = m.targetX - m.startX;
    int difY = m.targetY - m.startY;
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
                if (m.startY != 1)
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
                    return false;
                break;
            default:
                return false;
            }

        case 1:

            //Bauer geht nur einen nach vorne, bzw 2, wenn es das erste mal ist
            switch (difY)
            {
            case -1:
                if (figureChrTrgt != '\0')
                    return false;
                break;
            case -2:
                if (m.startY != 6)
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
                    return false;
                break;
            default:
                return false;
            }
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
            for (int i = m.startX; i < m.targetX; i++)
            {
                char c = getType(i, m.startY);
                if (c != '\0')
                    return false;
            }
        }
        else if (difX < 0)
        {
            for (int i = m.startX; i > m.targetX; i--)
            {
                char c = getType(i, m.startY);
                if (c != '\0')
                    return false;
            }
        }
        if (difY > 0)
        {
            for (int i = m.startY; i < m.targetY; i++)
            {
                char c = getType(i, m.startY);
                if (c != '\0')
                    return false;
            }
        }
        else if (difY < 0)
        {
            for (int i = m.startY; i > m.targetY; i--)
            {
                char c = getType(i, m.startY);
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

        if ((difX*difX) != (difY*difY))
            return false;
        
        //pr�fen ob weg frei ist diagonal
        //unten rechts
        if (difX > 0 && difY > 0)
        {
            for (int j = m.startY; j < m.targetY; j++)
            {
                for (int i = m.startX; i < m.targetX; i++)
                {
                    char c = getType(i, j);
                    if (c != '\0')
                        return false;
                }
            }
        }
        //oben links
        else if (difX < 0 && difY < 0)
        {
            for (int j = m.startY; j > m.targetY; j--)
            {
                for (int i = m.startX; i > m.targetX; i--)
                {
                    char c = getType(i, j);
                    if (c != '\0')
                        return false;
                }
            }
        }
        //oben rechts
        else if (difX > 0 && difY < 0)
        {
            for (int j = m.startY; j > m.targetY; j--)
            {
                for (int i = m.startY; i < m.targetY; i++)
                {
                    char c = getType(i, j);
                    if (c != '\0')
                        return false;
                }
            }
        }
        //unten links
        else if (difX < 0 && difY >0)
        {
            for (int j = m.startY; j > m.targetY; j++)
            {
                for (int i = m.startY; i > m.targetY; i--)
                {
                    char c = getType(i, j);
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
            return false;

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

        if (difX == 2 && difY == 1)
            break;
        if (difY == 2 && difX == 1)
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
                for (int j = m.startY; j < m.targetY; j++)
                {
                    for (int i = m.startX; i < m.targetX; i++)
                    {
                        char c = getType(i, j);
                        if (c != '\0')
                            return false;
                    }
                }
            }
            //oben links
            else if (difX < 0 && difY < 0)
            {
                for (int j = m.startY; j > m.targetY; j--)
                {
                    for (int i = m.startX; i > m.targetX; i--)
                    {
                        char c = getType(i, j);
                        if (c != '\0')
                            return false;
                    }
                }
            }
            //oben rechts
            else if (difX > 0 && difY < 0)
            {
                for (int j = m.startY; j > m.targetY; j--)
                {
                    for (int i = m.startY; i < m.targetY; i++)
                    {
                        char c = getType(i, j);
                        if (c != '\0')
                            return false;
                    }
                }
            }
            //unten links
            else if (difX < 0 && difY >0)
            {
                for (int j = m.startY; j > m.targetY; j++)
                {
                    for (int i = m.startY; i > m.targetY; i--)
                    {
                        char c = getType(i, j);
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
                for (int i = m.startX; i < m.targetX; i++)
                {
                    char c = getType(i, m.startY);
                    if (c != '\0')
                        return false;
                }
            }
            else if (difX < 0)
            {
                for (int i = m.startX; i > m.targetX; i--)
                {
                    char c = getType(i, m.startY);
                    if (c != '\0')
                        return false;
                }
            }
            if (difY > 0)
            {
                for (int i = m.startY; i < m.targetY; i++)
                {
                    char c = getType(i, m.startY);
                    if (c != '\0')
                        return false;
                }
            }
            else if (difY < 0)
            {
                for (int i = m.startY; i > m.targetY; i--)
                {
                    char c = getType(i, m.startY);
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


    return false;
}

std::list<Move> Chessposition::getValidMoves(int _startX, int _startY) const
{
    // Comment
    std::list<Move> moves;
    Move m;
    m.startX = _startX;
    m.startY = _startY;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            m.targetX = i;
            m.targetY = j;
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
    }

    return moves;
}

Move Chessposition::parseMove(std::string p_Move)
{
    Move m;
    
    //Capture abfangen
    if (p_Move[2] == 'x')
    {
        p_Move.replace(3,1,"");
        m.capture = false;
    }
    
    //Promotion abfangen
    if (p_Move.size() == 5)
    {
        m.promotion = p_Move[4];
        p_Move = p_Move.substr(0, 4);
    }


    //Start und Ziel Position als integer coodieren
    for (int i = 0; i < p_Move.size(); i++)
    {
        char c = p_Move[i];

        //Ziffer zu int 
        if (c > 48 && c < 58)
        {
            c = c - 49;
        }    

        //Buchstaben zu int
        if (c > 96 && c < 123)
        {
            c = c -97 ;
        }

        switch (i)
        {
        case 0:m.startX = c;
            break;

        case 1:m.startY = c;
            break;

        case 3:m.targetX = c;
            break;

        case 4:m.targetY = c;
            break;

        }
    }
    
    return m;
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
