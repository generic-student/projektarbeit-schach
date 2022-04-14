#include "uci_interface.hpp"

namespace engine::uci
{
    bool UciInterface::pollMessages(std::string* out_string)
    {
        if(m_messageBuffer.empty()) {
            return false;
        }

        *out_string = m_messageBuffer.front();
        m_messageBuffer.pop();

        return true;
    }
}