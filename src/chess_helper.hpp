#pragma once
#include <array>
#include <string>
#include <iostream>

namespace sm
{
    class ChessHelper
    {
    public:
        static void drawPositionInTerminal(const std::array<std::array<char, 8U>, 8U> &position)
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
    };
} // namespace sm
