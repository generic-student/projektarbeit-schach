#include "engine.hpp"


namespace engine
{
    Engine::Engine()
    {
        m_interface.initialize();
    }

    void Engine::start()
    {   
        m_isRunning = true;
        m_interface.activate();

        while(m_isRunning) {
            //get the messages from the gui
            std::string message;
            while(m_interface.pollMessages(&message)) {

            }

            //handle the messages (for example setting up the env)
        }
    }
}