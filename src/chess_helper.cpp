#include "chess_helper.hpp"
#include <regex>
namespace sm
{

    namespace ChessHelper
    {
        /**
         * @brief represents a char[8][8] array as a chess-field in the console
         * @details for debug purposes
         *
         * @param position char[8][8] array
         */
        void drawPositionInTerminal(const std::array<std::array<char, 8U>, 8U> &position)
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
        std::array<std::array<char, 8>, 8> fenToArray(const std::string &fen)
        {
            std::array<std::array<char, 8>, 8> pos;
            size_t row = 0;
            size_t col = 0;
            for (const char c : fen)
            {
                if (c <= '8' && c >= '1')
                {
                    for (size_t i = 0; i < (int)c - 48; i++)
                    {
                        pos[row][col++] = '\0';
                    }
                }
                else if (c == '/')
                {
                    row++;
                    col = 0;
                }
                else
                {
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
        std::string arrayToFen(const std::array<std::array<char, 8>, 8> &position)
        {
            std::string fen;
            for (size_t i = 0; i < 8; i++)
            {
                int emtpy_fields = 0;
                for (size_t j = 0; j < 8; j++)
                {
                    if (position[i][j] == '\0')
                    {
                        emtpy_fields++;
                    }
                    else
                    {
                        if (emtpy_fields != 0)
                        {
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
        Move parseMove(std::string move_str)
        {
            Move m;

            //remove the hyphen if included
            int hyphen_index = move_str.find("-");
            if(hyphen_index != std::string::npos) {
                move_str.replace(hyphen_index, 1, "");
            }

            int capture_index = move_str.find("x");
            if(capture_index != std::string::npos) {
                move_str.replace(capture_index, 1, "");
                m.capture = true;
            } else {
                m.capture = false;
            }

            // Capture abfangen
            if (move_str[2] == 'x')
            {
                move_str.replace(3, 1, "");
                m.capture = false;
            }

            // Promotion abfangen
            if (move_str.size() == 5)
            {
                m.promotion = move_str[4];
                move_str = move_str.substr(0, 4);
            }

            m.startCol = (char)move_str[0] - 'a';
            m.startRow = (char)move_str[1] - '1';
            m.targetCol = (char)move_str[2] - 'a';
            m.targetRow = (char)move_str[3] - '1';

            return m;
        }

        extern const std::array<const Move, 16> KNIGHT_MOVES = {
            Move(0, 0, 1, 2, false, 0, 0, '\0'),
            Move(0, 0, 2, 1, false, 0, 0, '\0'),
            Move(0, 0, -1, -2, false, 0, 0, '\0'),
            Move(0, 0, -2, -1, false, 0, 0, '\0'),
            Move(0, 0, -1, 2, false, 0, 0, '\0'),
            Move(0, 0, 2, -1, false, 0, 0, '\0'),
            Move(0, 0, -2, 1, false, 0, 0, '\0'),
            Move(0, 0, 1, -2, false, 0, 0, '\0'),
            Move(0, 0, 1, 2, true, 1, 2, '\0'),
            Move(0, 0, 2, 1, true, 2, 1, '\0'),
            Move(0, 0, -1, -2, true, -1, -2, '\0'),
            Move(0, 0, -2, -1, true, -2, -1, '\0'),
            Move(0, 0, -1, 2, true, -1, 2, '\0'),
            Move(0, 0, 2, -1, true, 2, -1, '\0'),
            Move(0, 0, -2, 1, true, -2, 1, '\0'),
            Move(0, 0, 1, -2, true, 1, -2, '\0')};

        extern const std::array<const Move, 36> PAWN_MOVES = {
            Move(0, 0, 1, 0, false, 0, 0, '\0'),    // einen nach unten
            Move(0, 0, 2, 0, false, 0, 0, '\0'),    // zwei nach unten
            Move(0, 0, -1, 0, false, 0, 0, '\0'),   // einen nach oben
            Move(0, 0, -2, 0, false, 0, 0, '\0'),   // zwei nach oben
            Move(0, 0, 1, 1, true, 1, 1, '\0'),     // diagonal unten rechts
            Move(0, 0, 1, -1, true, 1, -1, '\0'),   // diagonal unten links
            Move(0, 0, -1, 1, true, -1, 1, '\0'),   // diagonal oben rechts
            Move(0, 0, -1, -1, true, -1, -1, '\0'), // diagonal oben link

            Move(0, 0, 1, 1, true, 1, 0, '\0'),    // en passent diagonal unten rechts
            Move(0, 0, 1, -1, true, 1, 0, '\0'),   // en passent diagonal unten links
            Move(0, 0, -1, 1, true, -1, 0, '\0'),  // en passent diagonal oben rechts
            Move(0, 0, -1, -1, true, -1, 0, '\0'), // en passent diagonal oben link

            Move(0, 0, -1, 0, false, 0, 0, 'Q'), // promotion ohne capture oben
            Move(0, 0, -1, 0, false, 0, 0, 'R'),
            Move(0, 0, -1, 0, false, 0, 0, 'B'),
            Move(0, 0, -1, 0, false, 0, 0, 'N'),
            Move(0, 0, -1, 1, true, -1, 1, 'Q'), // promotion mit capture oben
            Move(0, 0, -1, 1, true, -1, 1, 'R'),
            Move(0, 0, -1, 1, true, -1, 1, 'B'),
            Move(0, 0, -1, 1, true, -1, 1, 'N'),
            Move(0, 0, -1, -1, true, -1, -1, 'Q'),
            Move(0, 0, -1, -1, true, -1, -1, 'R'),
            Move(0, 0, -1, -1, true, -1, -1, 'B'),
            Move(0, 0, -1, -1, true, -1, -1, 'N'),

            Move(0, 0, 1, 0, false, 0, 0, 'q'), // promotion ohne capture unten
            Move(0, 0, 1, 0, false, 0, 0, 'r'),
            Move(0, 0, 1, 0, false, 0, 0, 'b'),
            Move(0, 0, 1, 0, false, 0, 0, 'n'),
            Move(0, 0, 1, 1, true, 1, 1, 'q'), // promotin mit capture unten
            Move(0, 0, 1, 1, true, 1, 1, 'r'),
            Move(0, 0, 1, 1, true, 1, 1, 'b'),
            Move(0, 0, 1, 1, true, 1, 1, 'n'),
            Move(0, 0, 1, -1, true, 1, -1, 'q'),
            Move(0, 0, 1, -1, true, 1, -1, 'r'),
            Move(0, 0, 1, -1, true, 1, -1, 'b'),
            Move(0, 0, 1, -1, true, 1, -1, 'n')};

        extern const std::array<const Move, 56> BISHOP_MOVES = {
            Move(0, 0, 1, 1, false, 0, 0, '\0'), // diagonal rechts unten ohne capture
            Move(0, 0, 2, 2, false, 0, 0, '\0'),
            Move(0, 0, 3, 3, false, 0, 0, '\0'),
            Move(0, 0, 4, 4, false, 0, 0, '\0'),
            Move(0, 0, 5, 5, false, 0, 0, '\0'),
            Move(0, 0, 6, 6, false, 0, 0, '\0'),
            Move(0, 0, 7, 7, false, 0, 0, '\0'),
            Move(0, 0, 1, 1, true, 1, 1, '\0'), // diagonal rechts unten mit capture
            Move(0, 0, 2, 2, true, 2, 2, '\0'),
            Move(0, 0, 3, 3, true, 3, 3, '\0'),
            Move(0, 0, 4, 4, true, 4, 4, '\0'),
            Move(0, 0, 5, 5, true, 5, 5, '\0'),
            Move(0, 0, 6, 6, true, 6, 6, '\0'),
            Move(0, 0, 7, 7, true, 7, 7, '\0'),

            Move(0, 0, 1, -1, false, 0, 0, '\0'), // diagonal link unten ohne capture
            Move(0, 0, 2, -2, false, 0, 0, '\0'),
            Move(0, 0, 3, -3, false, 0, 0, '\0'),
            Move(0, 0, 4, -4, false, 0, 0, '\0'),
            Move(0, 0, 5, -5, false, 0, 0, '\0'),
            Move(0, 0, 6, -6, false, 0, 0, '\0'),
            Move(0, 0, 7, -7, false, 0, 0, '\0'),
            Move(0, 0, 1, -1, true, 1, -1, '\0'), // diagonal link unten mit capture
            Move(0, 0, 2, -2, true, 2, -2, '\0'),
            Move(0, 0, 3, -3, true, 3, -3, '\0'),
            Move(0, 0, 4, -4, true, 4, -4, '\0'),
            Move(0, 0, 5, -5, true, 5, -5, '\0'),
            Move(0, 0, 6, -6, true, 6, -6, '\0'),
            Move(0, 0, 7, -7, true, 7, -7, '\0'),

            Move(0, 0, -1, 1, false, 0, 0, '\0'), // diagonal rechts oben ohne capture
            Move(0, 0, -2, 2, false, 0, 0, '\0'),
            Move(0, 0, -3, 3, false, 0, 0, '\0'),
            Move(0, 0, -4, 4, false, 0, 0, '\0'),
            Move(0, 0, -5, 5, false, 0, 0, '\0'),
            Move(0, 0, -6, 6, false, 0, 0, '\0'),
            Move(0, 0, -7, 7, false, 0, 0, '\0'),
            Move(0, 0, -1, 1, true, -1, 1, '\0'), // diagonal rechts oben mit capture
            Move(0, 0, -2, 2, true, -2, 2, '\0'),
            Move(0, 0, -3, 3, true, -3, 3, '\0'),
            Move(0, 0, -4, 4, true, -4, 4, '\0'),
            Move(0, 0, -5, 5, true, -5, 5, '\0'),
            Move(0, 0, -6, 6, true, -6, 6, '\0'),
            Move(0, 0, -7, 7, true, -7, 7, '\0'),

            Move(0, 0, -1, -1, false, 0, 0, '\0'), // diagonal links oben ohne capture
            Move(0, 0, -2, -2, false, 0, 0, '\0'),
            Move(0, 0, -3, -3, false, 0, 0, '\0'),
            Move(0, 0, -4, -4, false, 0, 0, '\0'),
            Move(0, 0, -5, -5, false, 0, 0, '\0'),
            Move(0, 0, -6, -6, false, 0, 0, '\0'),
            Move(0, 0, -7, -7, false, 0, 0, '\0'),
            Move(0, 0, -1, -1, true, -1, -1, '\0'), // diagonal links oben mit capture
            Move(0, 0, -2, -2, true, -2, -2, '\0'),
            Move(0, 0, -3, -3, true, -3, -3, '\0'),
            Move(0, 0, -4, -4, true, -4, -4, '\0'),
            Move(0, 0, -5, -5, true, -5, -5, '\0'),
            Move(0, 0, -6, -6, true, -6, -6, '\0'),
            Move(0, 0, -7, -7, true, -7, -7, '\0')};

        extern const std::array<const Move, 20> KING_MOVES{
            Move(0, 0, 1, 0, false, 0, 0, '\0'),    // einen nach unten ohne capture
            Move(0, 0, -1, 0, false, 0, 0, '\0'),   // einen nach oben ohne capture
            Move(0, 0, 0, 1, false, 0, 0, '\0'),    // einen nach rechts ohne capture
            Move(0, 0, 0, -1, false, 0, 0, '\0'),   // einen nach links ohne capture
            Move(0, 0, 1, 1, true, 1, 1, '\0'),     // diagonal unten rechts ohne capture
            Move(0, 0, 1, -1, true, 1, -1, '\0'),   // diagonal unten links ohne capture
            Move(0, 0, -1, 1, true, -1, 1, '\0'),   // diagonal oben rechts ohne capture
            Move(0, 0, -1, -1, true, -1, -1, '\0'), // diagonal oben link ohne capture

            Move(0, 0, 1, 0, true, 0, 0, '\0'),     // einen nach unten mit capture
            Move(0, 0, -1, 0, true, 0, 0, '\0'),    // einen nach oben mit capture
            Move(0, 0, 0, 1, true, 0, 0, '\0'),     // einen nach rechts mit capture
            Move(0, 0, 0, -1, true, 0, 0, '\0'),    // einen nach links mit capture
            Move(0, 0, 1, 1, true, 1, 1, '\0'),     // diagonal unten rechts mit capture
            Move(0, 0, 1, -1, true, 1, -1, '\0'),   // diagonal unten links mit capture
            Move(0, 0, -1, 1, true, -1, 1, '\0'),   // diagonal oben rechts mit capture
            Move(0, 0, -1, -1, true, -1, -1, '\0'), // diagonal oben link mit capture

            Move(0, 0, 0, 2, false, 0, 0, '\0'), // rochade rechts
            Move(0, 0, 0, -2, false, 0, 0, '\0') // rochade links
        };

        extern const std::array<const Move, 58> ROOK_MOVES{
            Move(0, 0, 1, 0, false, 0, 0, '\0'), // nach unten ohne capture
            Move(0, 0, 2, 0, false, 0, 0, '\0'),
            Move(0, 0, 3, 0, false, 0, 0, '\0'),
            Move(0, 0, 4, 0, false, 0, 0, '\0'),
            Move(0, 0, 5, 0, false, 0, 0, '\0'),
            Move(0, 0, 6, 0, false, 0, 0, '\0'),
            Move(0, 0, 7, 0, false, 0, 0, '\0'),

            Move(0, 0, -1, 0, false, 0, 0, '\0'), // nach oben ohne capture
            Move(0, 0, -2, 0, false, 0, 0, '\0'),
            Move(0, 0, -3, 0, false, 0, 0, '\0'),
            Move(0, 0, -4, 0, false, 0, 0, '\0'),
            Move(0, 0, -5, 0, false, 0, 0, '\0'),
            Move(0, 0, -6, 0, false, 0, 0, '\0'),
            Move(0, 0, -7, 0, false, 0, 0, '\0'),

            Move(0, 0, 0, 1, false, 0, 0, '\0'), // nach rechts ohne capture
            Move(0, 0, 0, 2, false, 0, 0, '\0'),
            Move(0, 0, 0, 3, false, 0, 0, '\0'),
            Move(0, 0, 0, 4, false, 0, 0, '\0'),
            Move(0, 0, 0, 5, false, 0, 0, '\0'),
            Move(0, 0, 0, 6, false, 0, 0, '\0'),
            Move(0, 0, 0, 7, false, 0, 0, '\0'),

            Move(0, 0, 0, -1, false, 0, 0, '\0'), // nach links ohne capture
            Move(0, 0, 0, -2, false, 0, 0, '\0'),
            Move(0, 0, 0, -3, false, 0, 0, '\0'),
            Move(0, 0, 0, -4, false, 0, 0, '\0'),
            Move(0, 0, 0, -5, false, 0, 0, '\0'),
            Move(0, 0, 0, -6, false, 0, 0, '\0'),
            Move(0, 0, 0, -7, false, 0, 0, '\0'),

            Move(0, 0, 1, 0, true, 1, 0, '\0'), // nach unten mit capture
            Move(0, 0, 2, 0, true, 2, 0, '\0'),
            Move(0, 0, 3, 0, true, 3, 0, '\0'),
            Move(0, 0, 4, 0, true, 4, 0, '\0'),
            Move(0, 0, 5, 0, true, 5, 0, '\0'),
            Move(0, 0, 6, 0, true, 6, 0, '\0'),
            Move(0, 0, 7, 0, true, 7, 0, '\0'),

            Move(0, 0, -1, 0, true, -1, 0, '\0'), // nach oben mit capture
            Move(0, 0, -2, 0, true, -2, 0, '\0'),
            Move(0, 0, -3, 0, true, -3, 0, '\0'),
            Move(0, 0, -4, 0, true, -4, 0, '\0'),
            Move(0, 0, -5, 0, true, -5, 0, '\0'),
            Move(0, 0, -6, 0, true, -6, 0, '\0'),
            Move(0, 0, -7, 0, true, -7, 0, '\0'),

            Move(0, 0, 0, 1, true, 0, 1, '\0'), // nach rechts mit capture
            Move(0, 0, 0, 2, true, 0, 2, '\0'),
            Move(0, 0, 0, 3, true, 0, 3, '\0'),
            Move(0, 0, 0, 4, true, 0, 4, '\0'),
            Move(0, 0, 0, 5, true, 0, 5, '\0'),
            Move(0, 0, 0, 6, true, 0, 6, '\0'),
            Move(0, 0, 0, 7, true, 0, 7, '\0'),

            Move(0, 0, 0, -1, true, 0, -1, '\0'), // nach links mit capture
            Move(0, 0, 0, -2, true, 0, -2, '\0'),
            Move(0, 0, 0, -3, true, 0, -3, '\0'),
            Move(0, 0, 0, -4, true, 0, -4, '\0'),
            Move(0, 0, 0, -5, true, 0, -5, '\0'),
            Move(0, 0, 0, -6, true, 0, -6, '\0'),
            Move(0, 0, 0, -7, true, 0, -7, '\0'),

            Move(0, 0, 0, -2, false, 0, 0, '\0'), // rochade rechts
            Move(0, 0, 0, 3, false, 0, 0, '\0'),  // rochade links
        };

        std::string moveToString(sm::Move p_move)
        {
            std::string out = "";
            char c = p_move.startCol + 97;
            out += c;
            c = p_move.startRow + 49;
            out += c;
            //out += '-';
            c = p_move.targetCol + 97;
            out += c;
            c = p_move.targetRow + 49;
            out += c;
            c = p_move.promotion;
            out += ' ' + c;

            return out;
        }
    }
}