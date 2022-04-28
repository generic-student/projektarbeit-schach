#include "uci_command.hpp"
#include <iostream>

namespace sm
{
    namespace uci
    {
        /**
         * @brief Construct a new Command:: Command object
         * 
         * @param type 
         * @param args list of extra arguments appended to the main command
         */
        Command::Command(const Command::Type type, const std::vector<std::string> &args)
            : m_type(type), m_args(args)
        {
        }

        /**
         * @brief return the type of the command
         * 
         * @return Command::Type 
         */
        Command::Type Command::getType() const
        {
            return m_type;
        }

        /**
         * @brief set the type of the command
         * 
         * @param type 
         */
        void Command::setType(Command::Type type)
        {
            m_type = type;
        }
        
        /**
         * @brief return a list of the arguments
         * 
         * @return const std::vector<std::string>& 
         */
        const std::vector<std::string>& Command::getArgs() const
        {
            return m_args;
        }
        
        /**
         * @brief set the list of arguments that are appended to the main command
         * 
         * @param args 
         */
        void Command::setArgs(const std::vector<std::string>& args)
        {
            m_args = args;
        }
    }
}