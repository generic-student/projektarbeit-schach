#pragma once
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/function.hpp>
#include "tcp_connection.hpp"
#include <memory>

namespace sm::io
{
    class TcpServerAsync
    {
    public:
        TcpServerAsync(boost::asio::io_context &io_context, boost::asio::ip::port_type port_num);

        boost::asio::ip::port_type getPort() const;
        void setPort(boost::asio::ip::port_type port_num);

        void run();
        void setHandleReadCallback(TcpConnection::read_write_callback callback);
        void setHandleWriteCallback(TcpConnection::read_write_callback callback);

    private:
        void startAccept();

        void handleAccept(boost::shared_ptr<TcpConnection> new_connection,
                           const boost::system::error_code &error);

        boost::asio::io_context &m_ioContext;
        boost::asio::ip::tcp::acceptor m_acceptor;
        boost::asio::ip::port_type m_port;

        TcpConnection::read_write_callback m_handleReadCallback;
        TcpConnection::read_write_callback m_handleWriteCallback;
    };
}