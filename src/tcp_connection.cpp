#include "tcp_connection.hpp"
#include <boost/bind/bind.hpp>
#include <iostream>
#include <string>

namespace sm::io
{

    TcpConnection::TcpConnection(boost::asio::io_context &io_context)
        : m_socket(io_context)
    {
    }

    boost::shared_ptr<TcpConnection> TcpConnection::create(boost::asio::io_context &io_context)
    {
        return boost::shared_ptr<TcpConnection>(new TcpConnection(io_context));
    }

    void TcpConnection::startRead()
    {
        // Set a deadline for the read operation.
        // deadline_.expires_from_now(boost::posix_time::seconds(30));
        // Start an asynchronous operation to read a newline-delimited message.
        boost::asio::async_read_until(m_socket, m_inputBuffer, '\n',
                                      boost::bind(&TcpConnection::handleRead, shared_from_this(),
                                                  boost::asio::placeholders::error,
                                                  boost::asio::placeholders::bytes_transferred));
    }

    boost::asio::ip::tcp::socket &TcpConnection::getSocket()
    {
        return m_socket;
    }

    void TcpConnection::handleWrite(const boost::system::error_code &error, size_t bytes_transferred)
    {
        m_engine.handleUciWrite(error, bytes_transferred);
    }

    void TcpConnection::handleRead(const boost::system::error_code &error, std::size_t bytes_transferred)
    {
        //get the data that the client sent
        std::string data;
        if (!error)
        {
            std::istream is(&m_inputBuffer);
            std::getline(is, data);
        }

        //get the response from the engine
        std::string response = m_engine.handleUciRead(error, bytes_transferred, data);

        //send the response back to the client
        boost::asio::async_write(m_socket, boost::asio::buffer(response),
                                         boost::bind(&TcpConnection::handleWrite, shared_from_this(),
                                                     boost::asio::placeholders::error,
                                                     boost::asio::placeholders::bytes_transferred));

        //keep on reading new messages as long as the client has not quit and the engine is still running
        if(m_engine.isRunning() && !error) {
            startRead();
        } else {
            std::cout << "Error on receive: " << error.message() << "\n";

            m_socket.close();
        }

        //this scope contains legacy code TODO: remove
        {
        // if (!m_keepAlive)
        //     return;

        // if (!error)
        // {
        //     // Extract the newline-delimited message from the buffer.
        //     std::string line;
        //     std::istream is(&m_inputBuffer);
        //     std::getline(is, line);

        //     // Empty messages are heartbeats and so ignored.
        //     if (!line.empty())
        //     {
        //         std::cout << "Received: " << line << "\n";

        //         // TODO: answer the client
        //         std::string message = "no u\n";
        //         boost::asio::async_write(m_socket, boost::asio::buffer(message),
        //                                  boost::bind(&TcpConnection::handleWrite, shared_from_this(),
        //                                              boost::asio::placeholders::error,
        //                                              boost::asio::placeholders::bytes_transferred));
        //     }

        //     // continue to receive messages from the client
        //     // startRead();
        // }
        // else
        // {
        //     std::cout << "Error on receive: " << error.message() << "\n";

        //     // terminate the connection with the client
        //     terminate();
        // }
        }
    }

}