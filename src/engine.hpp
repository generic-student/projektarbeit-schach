#pragma once
#include <string>
#include <array>
#include "chessposition.hpp"
#include "engine_options.hpp"
#include "minMaxResult.hpp"

#define AVG_KING 4
#define AVG_QUEEN 13
#define AVG_PAWN 1
#define AVG_BISHOP 6
#define AVG_KNIGHT 4
#define AVG_ROOK 7
#define PA_IMPACT 2
#define CONNECTED_PAWNS 1.0f
#define DOUBLE_PAWNS -0.3f
#define ISOLATED_PAWNS -0.2f

namespace sm {
    /**
     * @brief The engine class contains all the functions and algorithms
     * for computing the best move and so on
     * 
     */
    class Engine {
        public:
            explicit Engine();
            virtual ~Engine() = default;

            const std::string getID() const;
            const std::string getAuthor() const;
            bool inDebugMode() const;
            void setDebugMode(bool debug);
            bool isReady() const;
            void setOption(const std::string& option, const std::string& value);
            const EngineOptions& getOptions() const;

            float max(const Chessposition& pos, int player, int depth, int desiredDepth, float alpha, float beta, int& nodes, Move* out_pMove) const;
            float min(const Chessposition& pos, int player, int depth, int desiredDepth, float alpha, float beta, int& nodes, Move* out_pMove) const;

            void findMove(const Chessposition& pos, Chessposition::Player player, int desiredDepth) const;
            /**
            * @brief Evaluate the board layout that is given to this function based on common chess evaluation rules.
            * 
            * @param currentBoard: The board layout that is to be evaluated.
            * 
            * @return float: Returns the evaluation value as a float.
            */
            float evaluateBoard(Chessposition& currentBoard);
            Chessposition& getPosition();
            const Chessposition& getPosition() const;

        private:
            const std::string m_id = "SchwachMatt 0.1";
            const std::string m_author = "No U";

            bool hasConnectedPawns(const char color, const unsigned short int p_row, const unsigned short int p_col, Chessposition& currentboard);
            bool hasDoublePawns(const char color, const unsigned short int p_row, const unsigned short int p_col, Chessposition& currentboard);
            bool hasIsolatedPawns(const char color, const unsigned short int p_col, Chessposition& currentboard);


            bool m_debugMode = false;
            bool m_ready = true;

            Chessposition m_position;
            EngineOptions m_engineOptions;
    };
}