#include "chess_helper.hpp"

namespace sm
{
    /**
     * @brief represents a char[8][8] array as a chess-field in the console
     * @details for debug purposes
     * 
     * @param position char[8][8] array
     */
    void ChessHelper::drawPositionInTerminal(const std::array<std::array<char, 8U>, 8U> &position)
    {
        for (size_t i = 0; i < 8U; i++)
        {
            std::cout << "|";
            for (size_t j = 0; j < 8U; j++)
            {
                if (position[i][j] == '\0')
                {
                    std::cout << " |";
                }
                else
                {
                    std::cout << position[i][j] << "|";
                }
            }
            std::cout << std::endl;
        }
    }
    
    /**
     * @brief converts a fen-string to a char[8][8] array
     * 
     * @param fen string in Forsyth-Edwards Notation
     * @return std::array<std::array<char, 8>, 8> 
     */
    std::array<std::array<char, 8>, 8> ChessHelper::fenToArray(const std::string& fen)
    {
        std::array<std::array<char, 8>, 8> pos;
        size_t row = 0;
        size_t col = 0;
        for(const char c : fen) {
            if(c <= '8' && c >= '1') {
                for (size_t i = 0; i < (int)c-48; i++)
                {
                    pos[row][col++] = '\0';
                }
            } else if(c == '/') {
                row++;
                col = 0;
            } else {
                pos[row][col++] = c;
            }
        }
        return pos;
    }
    
    /**
     * @brief generates a string in Forsyth-Edwards Notation from a char[8][8] array
     * 
     * @param position char[8][8] array
     * @return std::string Forsyth-Edwards Notation
     */
    std::string ChessHelper::arrayToFen(const std::array<std::array<char, 8>, 8>& position)
    {
        std::string fen;
        for (size_t i = 0; i < 8; i++)
        {
            int emtpy_fields = 0;
            for (size_t j = 0; j < 8; j++)
            {
                if(position[i][j] == '\0') {
                    emtpy_fields++;
                } 
                else {
                    if(emtpy_fields != 0) {
                        fen += std::to_string(emtpy_fields);
                        emtpy_fields = 0;
                    }
                    fen += position[i][j];
                }
            }
            fen += '/';
            
        }
        return fen;
    }

    /**
    * @brief convert a string in long algebraic notation to a move
    * 
    * @param move_str move in long algebraic notation
    * @return Move
    */
    Move ChessHelper::parseMove(std::string move_str)
    {
        Move m;

        //Capture abfangen
        if (move_str[2] == 'x')
        {
            move_str.replace(3, 1, "");
            m.capture = false;
        }

        //Promotion abfangen
        if (move_str.size() == 5)
        {
            m.promotion = move_str[4];
            move_str = move_str.substr(0, 4);
        }


        //Start und Ziel Position als integer coodieren
        for (int i = 0; i < move_str.size(); i++)
        {
            char c = move_str[i];

            //Ziffer zu int 
            if (c > 48 && c < 58)
            {
                c = c - 49;
            }

            //Buchstaben zu int
            if (c > 96 && c < 123)
            {
                c = c - 97;
            }

            switch (i)
            {
            case 0:m.startCol = c;
                break;

            case 1:m.startRow = c;
                break;

            case 3:m.targetCol = c;
                break;

            case 4:m.targetRow = c;
                break;

            }
        }

        return m;
    }

}