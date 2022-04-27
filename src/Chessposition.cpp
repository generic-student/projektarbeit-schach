#include "Chessposition.hpp"

bool Chessposition::isViableMove(std::string p_Move)
{
    Move m = parseMove(p_Move);
    
    if ((m.targetX > 7 || m.targetX < 0) || (m.targetY > 7 || m.targetY < 0))
    {
        return false;
    }

    return false;
}

std::list<Move> Chessposition::getValidMoves(int _startX, int _startY)
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

                    if (checkValidMove(m))
                    {
                        moves.push_back(m);
                    }
                }
            }
        }
    }

    return moves;
}


//   e1e2
//   e2xe3
//   e2e3
//   e7e8R
//   e7xe8Q R B N


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
            c = c - 48;
        }    

        //Buchstaben zu int
        if (c > 48 && c < 58)
        {
            c = c - 48;
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
