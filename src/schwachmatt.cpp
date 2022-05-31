#include "uci/uci.hpp"
#include <iostream>
#include "chess_helper.hpp"

#include <spdlog/spdlog.h>
#include "spdlog/sinks/basic_file_sink.h"

#include "benchmark.hpp"

int main(int argc, char const *argv[])
{
    auto logger = spdlog::basic_logger_mt("basic_logger", "logs/basic-log.log");
    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::info);
    logger->info("Debug logger setup done. \n");
    spdlog::set_default_logger(logger);

    //create an instance of a universal chess interface
    sm::uci::UniversalChessInterface interface;

    //start handling messages
    interface.start();

    // Benchmark benchmark("logs/benchmarks/getvalidmoves.log", "Get Valid Moves Parallel", "benchmark");
    // sm::Chessposition cp;
    // sm::Engine engine;

    // benchmark.start_test();
    // for(int j = 1; j <= 10000; j*=10) {
    //     benchmark.begin();
    //     for(int i = 0; i < j; i++) {
    //         cp.getValidMoves(true, true);
    //     }
    //     benchmark.end("Spieler {1:+d}    Elapsed: {0:08.3f} s    Iterations: {2:05d}", 1, j);
    // }

    // cp.applyMove(sm::ChessHelper::parseMove("b2b3"), false);
    // for(int j = 1; j <= 10000; j*=10) {
    //     benchmark.begin();
    //     for(int i = 0; i < j; i++) {
    //         cp.getValidMoves(true, true);
    //     }
    //     benchmark.end("Spieler {1:+d}    Elapsed: {0:08.3f} s    Iterations: {2:05d}", -1, j);
    // }
    // benchmark.end_test();

    return 0;
}
