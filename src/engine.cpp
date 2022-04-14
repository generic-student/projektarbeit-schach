#include "engine.hpp"
#include <iostream>

namespace sm
{
    constexpr const unsigned int DEFAULT_PORT = 3000;

    Engine::Engine()
        : m_ioContext()
    {
        //create an instance of the tcp server
        m_pTcpServer = std::make_unique<sm::io::TcpServerAsync>(m_ioContext, DEFAULT_PORT);

        //set the read-callback
        m_pTcpServer->setHandleReadCallback(boost::bind(
            &Engine::handleUciRead,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred,
            boost::placeholders::_3
            ));
        
        //set the write callback
        m_pTcpServer->setHandleWriteCallback(boost::bind(
            &Engine::handleUciWrite,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred,
            boost::placeholders::_3
            ));
    }

    void Engine::start()
    {   
        m_isRunning = true;

        m_pTcpServer->run();
    }
    
    void Engine::handleUciRead(const boost::system::error_code& ec , size_t bytes_transferred, const std::string& data)
    {
        
        if(ec) return;

        std::cout << "message received: " << data << std::endl;
    }
    
    void Engine::handleUciWrite(const boost::system::error_code& ec , size_t bytes_transferred, const std::string& data)
    {
        if(ec) return;

        std::cout << "message sent!" << std::endl;
    }
}