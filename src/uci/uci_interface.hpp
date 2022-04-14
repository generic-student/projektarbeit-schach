#pragma once
#include <queue>
#include <string>

namespace engine::uci
{
    /**
     * @brief Handles all aspects of the Uci Interface and manages the server that
     * holds the connection to the client. It exposes functions that can be used to 
     * get the current messages.
     * 
     */
    class UciInterface {
        public:
            explicit UciInterface() = default;
            virtual ~UciInterface() = default;

            void initialize();
            void activate();

            std::string getMessage();

            bool pollMessages(std::string* out_string);

        private:
            std::queue<std::string> m_messageBuffer;
    };
} // namespace engine::uci
