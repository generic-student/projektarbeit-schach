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


    // std::vector<std::string> fen_strings;
    // for(int i = 0; i < 100; ++i) {
    //     std::string available_pieces = "QRRBBNNPPPPPPPPqrrbbnnpppppppp";
    //     std::random_shuffle(available_pieces.begin(), available_pieces.end());

    //     //add the string up to 64 chars; and shuffle
    //     std::string fen_raw = available_pieces.substr(0, 14);
    //     //add both kings
    //     fen_raw += "kK";
    //     fen_raw.insert(0,48,'\0');
    //     std::random_shuffle(fen_raw.begin(), fen_raw.end());
        
    //     std::string fen;
    //     for(size_t row = 0; row < 8; row++) {
    //         int count = 0;
    //         for (size_t column = 0; column < 8; column++)
    //         {
    //             if(fen_raw.at(row * 8 + column) == '\0') {
    //                 count++;
    //             } else {
    //                 if(count != 0) {
    //                     fen+=std::to_string(count);
    //                     count = 0;
    //                 }
    //                 fen += fen_raw.at(row * 8 + column);
    //             }
    //         }
    //         if(count != 0) fen += std::to_string(count);
    //         if(row != 7) fen += '/';
    //     }

    //     fen += (i < 50) ? " w - - 0 1" : " w - - 0 1";
    //     fen_strings.push_back(fen);
    // }

    // Benchmark benchmark("logs/benchmarks/minmax/minmax.csv", "IKAINM", "benchmark", false);
    // sm::Chessposition cp;
    // sm::Engine engine;

    // benchmark.start_test("Test;Player;Elapsed Time (s);Iterations");
    // const int ITERATIONS = 1;
    // for(auto& fen : fen_strings) {
    //     cp.setFEN(fen);
    //     benchmark.begin();
    //     for(int i = 0; i < ITERATIONS; i++) {
    //         engine.findMove(cp, sm::Chessposition::WHITE, 3);
    //     }
    //     benchmark.end("findMove always-copy;{1:s};{0:08.3f};{2:05d}", "White", ITERATIONS);
    // }

    // cp.applyMove(sm::ChessHelper::parseMove("b2b3"), false);
    // for(int j = 0; j <= 10000; j+=100) {
    //     benchmark.begin();
    //     for(int i = 0; i < j; i++) {
    //         cp.getValidMoves(true, true);
    //     }
    //     benchmark.end("Get Valid Moves Bruteforce;{1:s};{0:08.3f};{2:05d}", "Black", j);
    // }

    return 0;
}
