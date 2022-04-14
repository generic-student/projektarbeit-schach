#pragma once
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>

namespace sm::io
{
    class TcpConnection : public boost::enable_shared_from_this<TcpConnection>
    {
    public:
        typedef boost::function<void(const boost::system::error_code&, size_t, std::string)> read_write_callback;

        static boost::shared_ptr<TcpConnection> create(boost::asio::io_context &io_context);

        boost::asio::ip::tcp::socket &getSocket();

        void startRead();
        void setHandleReadCallback(read_write_callback callback);
        void setHandleWriteCallback(read_write_callback callback);

        void terminate();

    private:
        TcpConnection(boost::asio::io_context &io_context);

        void handleWrite(const boost::system::error_code & error, std::size_t bytes_transferred);
        void handleRead(const boost::system::error_code& error, std::size_t bytes_transferred);

        boost::asio::ip::tcp::socket m_socket;
        boost::asio::streambuf m_inputBuffer;
        read_write_callback m_handleReadCallback;
        read_write_callback m_handleWriteCallback;
    };
}