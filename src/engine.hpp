#pragma once
#include <string>
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

        private:
            const std::string m_id = "SchwachMatt 0.1";
            const std::string m_author = "No U";
    };
}