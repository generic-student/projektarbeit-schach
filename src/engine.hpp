#pragma once
#include "tcp_server_async.hpp"
#include <boost/enable_shared_from_this.hpp>
#include <string>
#include <memory>
namespace sm {
    class Engine : public boost::enable_shared_from_this<Engine>{
        public:
            explicit Engine();
            virtual ~Engine() = default;

            void start();

        private:
            void handleUciRead(const boost::system::error_code& ec, size_t bytes_transferred, const std::string& data );
            void handleUciWrite(const boost::system::error_code & , size_t bytes_transferred, const std::string& data );

        private:
            bool m_isRunning = false;
            bool m_isReady = false;
            bool m_isInitialized = false;

            boost::asio::io_context m_ioContext;
            std::unique_ptr<sm::io::TcpServerAsync> m_pTcpServer;
    };
}