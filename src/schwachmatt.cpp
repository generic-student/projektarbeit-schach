#include "uci/uci.hpp"
#include <chrono>
#include <iostream>
#include "chess_helper.hpp"

int main(int argc, char const *argv[])
{
    //create an instance of a universal chess interface
    sm::uci::UniversalChessInterface interface;

    //start handling messages
    //interface.start();

    sm::Chessposition cp;

    std::list<sm::Move> allMoves;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i=0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            std::list<sm::Move> l = cp.getValidMoves(i, j);
            allMoves.splice(allMoves.end(),l);
        }
    }
    
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    // To get the value of duration use the count()
    // member function on the duration object
    std::cout << duration.count() << std::endl;
    std::cout << allMoves.size() << std::endl;

    for (auto a : allMoves)
    {
        std::cout << sm::ChessHelper::moveToString(a) << std::endl;
    }

    return 0;
}
