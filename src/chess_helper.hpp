#pragma once
#include <array>
#include <string>
#include <iostream>

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
    };

} // namespace sm
