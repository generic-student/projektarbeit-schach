#include "tcp_connection.hpp"
#include <iostream>

namespace api::io
{
    boost::shared_ptr<TcpConnection> TcpConnection::create(boost::asio::io_context &io_context)
    {
        return boost::shared_ptr<TcpConnection>(new TcpConnection(io_context));
    }

    boost::asio::ip::tcp::socket &TcpConnection::getSocket()
    {
        return m_socket;
    }

    void TcpConnection::startRead()
    {
        // Set a deadline for the read operation.
        // deadline_.expires_from_now(boost::posix_time::seconds(30));

        // Start an asynchronous operation to read a newline-delimited message.
        boost::asio::async_read_until(m_socket, m_inputBuffer, '\n',
                                      boost::bind(&TcpConnection::handleRead, shared_from_this(),
                                                  boost::asio::placeholders::error));
    }
    
    void TcpConnection::terminate()
    {
        m_keepAlive = false;
        m_socket.close();
    }

    void TcpConnection::start()
    {
        startRead();
    }

    TcpConnection::TcpConnection(boost::asio::io_context &io_context)
        : m_socket(io_context)
    {
    }

    void TcpConnection::handleWrite(const boost::system::error_code & /*error*/,
                                    size_t /*bytes_transferred*/)
    {
    }

    void TcpConnection::handleRead(const boost::system::error_code &ec)
    {
        if(!m_keepAlive) return;

        if (!ec)
        {
            // Extract the newline-delimited message from the buffer.
            std::string line;
            std::istream is(&m_inputBuffer);
            std::getline(is, line);

            // Empty messages are heartbeats and so ignored.
            if (!line.empty())
            {
                std::cout << "Received: " << line << "\n";

                //TODO: answer the client
                std::string message = "no u\n";
                boost::asio::async_write(m_socket, boost::asio::buffer(message), 
                                         boost::bind(&TcpConnection::handleWrite, shared_from_this(),
                                                      boost::asio::placeholders::error,
                                                      boost::asio::placeholders::bytes_transferred));

            }

            //continue to receive messages from the client
            startRead();
        }
        else
        {
            std::cout << "Error on receive: " << ec.message() << "\n";

            //terminate the connection with the client
            terminate();
        }
    }

}