#pragma once
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>

namespace api::io
{
    class TcpConnection : public boost::enable_shared_from_this<TcpConnection>
    {
    public:
        static boost::shared_ptr<TcpConnection> create(boost::asio::io_context &io_context);

        boost::asio::ip::tcp::socket &getSocket();

        void start();
        void startRead();

        void terminate();

    private:
        TcpConnection(boost::asio::io_context &io_context);

        void handleWrite(const boost::system::error_code & /*error*/,
                          size_t /*bytes_transferred*/);

        void handleRead(const boost::system::error_code& ec);

        boost::asio::ip::tcp::socket m_socket;
        boost::asio::streambuf m_inputBuffer;

        bool m_keepAlive = true;
    };
}