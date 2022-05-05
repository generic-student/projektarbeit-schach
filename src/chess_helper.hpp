#pragma once
#include <array>
#include <string>
#include <iostream>
#include "move.hpp"

namespace sm
{
    namespace ChessHelper
    {

        void drawPositionInTerminal(const std::array<std::array<char, 8U>, 8U> &position);
        std::array<std::array<char, 8>, 8> fenToArray(const std::string& fen);
        std::string arrayToFen(const std::array<std::array<char, 8>, 8>& position);
        Move parseMove(std::string move_str);
        
        extern const std::array<const Move, 16> KNIGHT_MOVES;
        extern const std::array<const Move, 36> PAWN_MOVES;
        extern const std::array<const Move, 56> BISHOP_MOVES;
        extern const std::array<const Move, 20> KING_MOVES;
        extern const std::array<const Move, 58> ROOK_MOVES;

    };

} // namespace sm
