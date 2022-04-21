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
}