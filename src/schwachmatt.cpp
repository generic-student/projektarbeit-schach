#include "uci/uci.hpp"
#include <chrono>
#include <iostream>
#include "chess_helper.hpp"

#include <spdlog/spdlog.h>
#include "spdlog/sinks/basic_file_sink.h"

int main(int argc, char const *argv[])
{
    auto logger = spdlog::basic_logger_mt("basic_logger", "logs/basic-log.txt");
    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::info);
    logger->info("Debug logger setup done. \n");
    spdlog::set_default_logger(logger);

    //create an instance of a universal chess interface
    sm::uci::UniversalChessInterface interface;

    //start handling messages
    interface.start();

    // sm::Chessposition cp;

    // std::vector<sm::Move> allMoves;

    // auto start = std::chrono::high_resolution_clock::now();

    // allMoves = cp.getValidMoves();
    
    // auto stop = std::chrono::high_resolution_clock::now();

    // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    // // To get the value of duration use the count()
    // // member function on the duration object
    // std::cout << duration.count() << std::endl;
    // std::cout << allMoves.size() << std::endl;

    // for (auto a : allMoves)
    // {
    //     std::cout << sm::ChessHelper::moveToString(a) << std::endl;
    // }

    //cp.generateThreatMap();

    return 0;
}
