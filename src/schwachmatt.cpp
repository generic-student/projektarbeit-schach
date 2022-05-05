#include "uci/uci.hpp"
#include <chrono>
#include <iostream>

int main(int argc, char const *argv[])
{
    //create an instance of a universal chess interface
    //sm::uci::UniversalChessInterface interface;

    //start handling messages
    //interface.start();

    sm::Chessposition pos;
    std::list<sm::Move> moves;

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            moves = pos.getValidMoves(i, j);
            std::cout << "row: " << i << ", col: " << j << " -> " << moves.size() << std::endl; 
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << std::endl;
    

    return 0;
}
