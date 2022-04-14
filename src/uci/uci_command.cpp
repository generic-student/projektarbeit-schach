#include "uci_command.hpp"

namespace engine::uci
{
    Command::Command(const Command::Type type, const std::map<std::string, std::string>& args)
        : m_type(type), m_args(args)
    {
        
    }
    
    Command::Type Command::getType() const
    {
        return m_type;
    }
    
    void Command::setType(Command::Type type)
    {
        m_type = type;
    }
}