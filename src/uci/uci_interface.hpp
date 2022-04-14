// #pragma once
// #include <queue>
// #include <string>
// #include "../tcp_server_async.hpp"

// namespace sm::uci
// {
//     /**
//      * @brief Handles all aspects of the Uci Interface and manages the server that
//      * holds the connection to the client. It exposes functions that can be used to 
//      * get the current messages.
//      * 
//      */
//     class UciInterface {
//         public:
//             explicit UciInterface(int port);
//             virtual ~UciInterface() = default;

//             void initialize();
//             void activate();

//             std::string getMessage();

//             bool pollMessages(std::string* out_string);

//         private:
//             io::TcpServerAsync m_tcpServer;
//             std::queue<std::string> m_messageBuffer;
//     };
// } // namespace engine::uci
