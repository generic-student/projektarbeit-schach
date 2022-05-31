#pragma once
#include <spdlog/spdlog.h>
#include "spdlog/sinks/basic_file_sink.h"
#include <chrono>
#include <string>

/**
 * @brief Static class for benchmarking and logging the results
 * 
 */
class Benchmark {
    public:
        explicit Benchmark(const std::string& filename, const std::string& testname, const std::string& loggername) 
            : name(testname)
        {
            m_pLogger = spdlog::basic_logger_mt(loggername, filename);
            m_pLogger->set_level(spdlog::level::info);
            m_pLogger->flush_on(spdlog::level::info);
            m_pLogger->set_pattern("[%l] %v");
        }

        virtual ~Benchmark() {}

        void start_test() {
            m_pLogger->info("**************************************************************");
            m_pLogger->info("Test: {0}", name);
            m_pLogger->info("**************************************************************");
        }
        
        void end_test() {
            m_pLogger->info("**************************************************************");
        }

        void begin() {
            m_timepoint = std::chrono::high_resolution_clock::now();
        }

        void end() {
            auto now = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_timepoint);
            m_pLogger->info("Elapsed: {0:05.3f} s", elapsed.count());
        }

        template<typename... Args>
        void end(fmt::format_string<Args...> fmt, Args &&...args) {
            auto now = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_timepoint);
            double seconds = static_cast<double>(elapsed.count()) / 1000.0; 
            m_pLogger->info(fmt, seconds, std::forward<Args>(args)...);
        }

        void setTestName(const std::string testname) {
            name = testname;
        }

    private:
        std::shared_ptr<spdlog::logger> m_pLogger;
        std::chrono::high_resolution_clock::time_point m_timepoint;
        std::string name;

};