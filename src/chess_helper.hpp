#pragma once
#include <array>
#include <string>
#include <iostream>
#include "move.hpp"

namespace sm
{
    class ChessHelper
    {
    private:
        explicit ChessHelper() = default;
        virtual ~ChessHelper() = default;

    public:
        static void drawPositionInTerminal(const std::array<std::array<char, 8U>, 8U> &position);
        static std::array<std::array<char, 8>, 8> fenToArray(const std::string& fen);
        static std::string arrayToFen(const std::array<std::array<char, 8>, 8>& position);
        static Move parseMove(std::string move_str);
        static std::string moveToString(sm::Move p_move);
        
    };

} // namespace sm
