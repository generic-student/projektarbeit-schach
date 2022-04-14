#include "tcp_server_async.hpp"
#include <iostream>
#include "uci.hpp"

using boost::asio::ip::tcp;

namespace sm::io
{
    TcpServerAsync::TcpServerAsync(boost::asio::io_context &io_context, boost::asio::ip::port_type port_num)
        : m_ioContext(io_context),
          m_acceptor(io_context, tcp::endpoint(tcp::v4(), port_num)),
          m_port(port_num)
    {
    }
    
    void TcpServerAsync::run()
    {
        std::cout << "Started local server on port " << m_port << std::endl;
        startAccept();

        //start the async execution
        m_ioContext.run();
    }
    
    void TcpServerAsync::setHandleReadCallback(TcpConnection::read_write_callback callback)
    {
        m_handleReadCallback = callback;
    }
    
    void TcpServerAsync::setHandleWriteCallback(TcpConnection::read_write_callback callback)
    {
        m_handleWriteCallback = callback;
    }

    /**
     * @brief start accepting connections asynchronously
     * 
     */
    void TcpServerAsync::startAccept()
    {
        boost::shared_ptr<TcpConnection> new_connection =
            TcpConnection::create(m_ioContext);

        if(!m_handleReadCallback.empty())
            new_connection->setHandleReadCallback(m_handleReadCallback);
        if(!m_handleWriteCallback.empty())
            new_connection->setHandleWriteCallback(m_handleWriteCallback);

        m_acceptor.async_accept(new_connection->getSocket(),
                               boost::bind(&TcpServerAsync::handleAccept, this, new_connection,
                                           boost::asio::placeholders::error));
    }

    void TcpServerAsync::handleAccept(boost::shared_ptr<TcpConnection> new_connection,
                                   const boost::system::error_code &error)
    {
        if (!error)
        {
            std::cout << "A new device has connected!" << std::endl;
            new_connection->startRead();
        }

        startAccept();
    }
}