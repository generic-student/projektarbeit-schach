#pragma once
#include <string>
#include <array>
#include "chessposition.hpp"
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
            /**
            * @brief Evaluate the board layout that is given to this function based on common chess evaluation rules.
            * 
            * @param currentBoard: The board layout that is to be evaluated.
            * 
            * @return float: Returns the evaluation value as a float.
            */
            float evaluateBoard(const std::array<std::array<char, 8>, 8>& currentBoard);
            Chessposition& getPosition();
            const Chessposition& getPosition() const;

        private:
            const std::string m_id = "SchwachMatt 0.1";
            const std::string m_author = "No U";

            bool m_debugMode = false;
            bool m_ready = true;

            Chessposition m_position;
    };
}