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
