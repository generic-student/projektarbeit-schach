#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace sm {
    struct EngineOption {
        bool isSupported;
        std::string value;
        std::string default_value;
        std::string type;
    };

    class EngineOptions {
        public:
            //constructor/destructor
            explicit EngineOptions();
            virtual ~EngineOptions();

            bool isSupported(const std::string& id) const;
            void setValue(const std::string& id, const std::string& value);
            const std::string& getValue(const std::string& id) const;

            const EngineOption& getOption(const std::string& id) const;
            const std::unordered_map<std::string, EngineOption>& getEngineOptions() const;

        private:
            //maps option-id to option
            std::unordered_map<std::string, EngineOption> m_engineOptions;
    };
}