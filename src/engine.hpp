#pragma once
#include <string>
#include <array>
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
            /**
            * @brief Evaluate the board layout that is given to this function based on common chess evaluation rules.
            * 
            * @param currentBoard: The board layout that is to be evaluated.
            * 
            * @return float: Returns the evaluation value as a float.
            */
            float evaluateBoard(std::array<std::array<char, 8>, 8> currentBoard);

        private:
            const std::string m_id = "SchwachMatt 0.1";
            const std::string m_author = "No U";
    };
}