#pragma once
#include <boost/asio.hpp>
namespace sm {
    class Engine {
        public:
            explicit Engine();
            virtual ~Engine() = default;

            std::string handleUciRead(const boost::system::error_code& ec, size_t bytes_transferred, const std::string& data );
            std::string handleUciWrite(const boost::system::error_code & , size_t bytes_transferred);

            bool isRunning() const { return m_isRunning; }
            void setIsRunning(bool isRunning) { m_isRunning = isRunning; }

            bool isReady() const { return m_isReady; }
            void setIsReady(bool isReady) { m_isReady = isReady; }

            bool isInitialized() const { return m_isInitialized; }
            void setIsInitialized(bool isInitialized) { m_isInitialized = isInitialized; }

        private:
            bool m_isRunning = false;
            bool m_isReady = false;
            bool m_isInitialized = false;

    };
}