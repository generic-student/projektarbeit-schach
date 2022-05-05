#include "uci/uci.hpp"
#include <chrono>
#include <iostream>
#include "chess_helper.hpp"

int main(int argc, char const *argv[])
{
    //create an instance of a universal chess interface
    //sm::uci::UniversalChessInterface interface;

    //start handling messages
    //interface.start();

    sm::Chessposition cp;

    std::list<sm::Move> allMoves;

    auto start = std::chrono::high_resolution_clock::now();

    allMoves = cp.getValidMoves();
    
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

    cp.generateThreatMap();

    return 0;
}
