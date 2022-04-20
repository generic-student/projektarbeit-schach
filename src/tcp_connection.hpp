#pragma once
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "engine.hpp"

namespace sm
{
    namespace io
    {
        class TcpConnection : public boost::enable_shared_from_this<TcpConnection>
        {
        public:
            static boost::shared_ptr<TcpConnection> create(boost::asio::io_context &io_context);

            void startRead();
            boost::asio::ip::tcp::socket &getSocket();

        private:
            explicit TcpConnection(boost::asio::io_context &io_context);

        protected:
            void handleWrite(const boost::system::error_code &error, std::size_t bytes_transferred);
            void handleRead(const boost::system::error_code &error, std::size_t bytes_transferred);

            // represents the tcp-socket that hold the connection to the client
            boost::asio::ip::tcp::socket m_socket;

            // buffer that stores the messages that the client sends
            boost::asio::streambuf m_inputBuffer;

            // instance of a chess engine
            sm::Engine m_engine;
        };
    }
}