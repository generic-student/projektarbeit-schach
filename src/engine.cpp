#include "engine.hpp"

namespace sm
{
    Engine::Engine()
    {
    }
    
    const std::string Engine::getID() const
    {
        return m_id;
    }
    
    const std::string Engine::getAuthor() const
    {
        return m_author;
    }
    
    bool Engine::inDebugMode() const
    {
        return m_debugMode;
    }
    
    void Engine::setDebugMode(bool debug)
    {
        m_debugMode = debug;
    }
    
    bool Engine::isReady() const
    {
        return m_ready;
    }
    
    void Engine::setOption(const std::string& option, const std::string& value)
    {
        throw "Invalid Option";
    }
}