#include "Chessposition.hpp"

std::string Chessposition::getFEN()
{
    return std::string();
}

char Chessposition::getType(int x, int y)
{
    return m_Position[x][y];
}

int Chessposition::getActivePlayer()
{
    return 0;
}

int Chessposition::getMoveNumber()
{
    return 0;
}

float Chessposition::getPositionEvaluation()
{
    return 0.0f;
}

bool Chessposition::isViableMove(std::string p_Move)
{
    Move m = parseMove(p_Move);
    int type = 0;
    char figureChr;
    char figureChrTrgt;

    //Ausfiltern wenn Move außerhalb des Feldes ist
    if ((m.targetX > 7 || m.targetX < 0) || (m.targetY > 7 || m.targetY < 0))
    {
        return false;
    }
    //Ausfiltern wenn start Position außerhalb des Feldes ist
    if ((m.startX > 7 || m.startX < 0) || (m.startY > 7 || m.startY < 0))
    {
        return false;
    }

    figureChr = getType(m.startX, m.startY);
    figureChrTrgt = getType(m.targetX, m.targetY);

    //überprüfen ob an der Prosition eine Figur des aktiven Spielers steht
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

    //Überprüfen, ob der Move für die jeweilige Figur gältig ist

    int difX = m.targetX - m.startX;
    int difY = m.targetY - m.startY;
    switch (type)
    {
        /*
            Bauern Logik

            zuerst prüfen welcher Spieler an der reihe ist.

            wenn er diagonal geht prüfen
            Danach prüfen ob der Bauer gerade oder diagonal geht (x Koordinate ändert sich),
            Dann überprüfen, ob der Bauer genau ein Feld Diagonal geht (x und y Koordinate genau 1 unterschied, für y Koordinate je nach Farbe +1 oder -1)
            zuletzt überprüfen ob auf dem Feld eine Figur der anderen Farbe steht.

            Für gerade prüfen ob es eine oder zwei Felder nach vorne geht (Y-Koordinate 1 oder 2 geändert)

            Wenn es 2 sind prüfen ob es der erste move dieses Bauern ist

            wenn es einen nach vorne geht prüfen, ob das Feld frei ist.


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
                //Prüfen ob gegnerische figur auf dem feld 
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

            prüfen ob der Turm nur gerade geht (entweder x oder y Koordinate bleibt gleich)

            dann Prüfen, ob der weg frei ist (Koordinate um 1 erhöhen und prüfen, ob das Feld frei ist)

            zuletz prüfen, ob das Ziel feld frei ist, oder eine Figur der anderen Farbe drauf ist
             */
    case 2:
        if (difX != 0 && difY != 0)
            return false;

        //prüfen ob der weg frei ist gerade
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
        
        //prüfen, dass auf dem Zielfeld keine eigene figur steht
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

            Läufer Logik

           prüfen ob der Läufer diagonal geht (x und y Veränderung ist gleich)

           danach prüfen ob der Weg zum ziel frei ist (Sowohl x und y koordinate immer um 1 dem ziel annähern und prüfen ob das feld frei ist)

           zuletzt prüfen, ob das ziel Feld Frei ist, oder eine Figur der anderen Farbe drauf ist

            */
    case 3:

        if ((difX*difX) != (difY*difY))
            return false;
        
        //prüfen ob weg frei ist diagonal
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


        //prüfen, dass auf dem Zielfeld keine eigene figur steht
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
            König Logik

            prüfen ob ob der König maximal ein feld geht (prüfen ob x und y Veränderung <=1 )

             */
    case 4:

        if ((difX * difX) > 1 || (difY * difY) > 1)
            return false;

        //prüfen, dass auf dem Zielfeld keine eigene figur steht
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

            prüfen ob der Springer ein Koordinate um 2 und die andere um 1 verändert
            danach prüfen, ob das Zielfeld leer ist, oder eine Figur der andern Farbe drauf ist.

            */
    case 5:

        //prüfen, dass auf dem Zielfeld keine eigene figur steht
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

            Königin Logik

            Läufer, oder Turm muss passen, also nicht beide falsch

            */
    case 6:

        //prüfen ob sie Diagonal oder gerade geht, abhängig davon dann Turm obder Läufer logik wählen
        //Läufer
        if ((difX * difX) == (difY * difY))
        {

            //prüfen ob weg frei ist diagonal
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


            //prüfen, dass auf dem Zielfeld keine eigene figur steht
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
            //prüfen ob der weg frei ist gerade
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

            //prüfen, dass auf dem Zielfeld keine eigene figur steht
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


//   e1e2
//   e2xe3
//   e2e3
//   e7e8R
//   e7xe8Q R B N


void Chessposition::FENToArray(std::string)
{
}

std::string Chessposition::arrayToFEN()
{
    return std::string();
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
