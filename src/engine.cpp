#include "engine.hpp"
#include <iostream>

namespace sm
{
    Engine::Engine()
    {
        //TODO: set the flags via the uci interface
        m_isRunning = true;
        m_isInitialized = true;
        m_isReady = true; 
    }
    
    /**
     * @brief handles the data that came from the client and returns a reponse
     * 
     * @param ec error code
     * @param bytes_transferred 
     * @param data message sent by the client
     * @return std::string response from the engine (in uci format)
     */
    std::string Engine::handleUciRead(const boost::system::error_code& ec , size_t bytes_transferred, const std::string& data)
    {
        
        if(ec) return "";

        std::cout << "message received: " << data << std::endl;

        return "success";
    }
    
    /**
     * @brief handles the response after sending a message to the client
     * 
     * @param ec error code
     * @param bytes_transferred 
     * @return std::string response from the engine (in uci format)
     */
    std::string Engine::handleUciWrite(const boost::system::error_code& ec , size_t bytes_transferred)
    {
        if(ec) return "";

        std::cout << "message sent!" << std::endl;

        return "";
    }
}