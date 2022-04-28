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

}