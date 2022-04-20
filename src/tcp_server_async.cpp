#include "tcp_server_async.hpp"
#include <iostream>

using boost::asio::ip::tcp;

namespace sm
{
    namespace io
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

            // start the async execution
            m_ioContext.run();
        }

        /**
         * @brief start accepting connections asynchronously
         *
         */
        void TcpServerAsync::startAccept()
        {
            boost::shared_ptr<TcpConnection> new_connection = TcpConnection::create(m_ioContext);

            // wait for other connections
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
}