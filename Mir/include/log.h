#pragma once

#include "core.h"
#include <spdlog/spdlog.h>

namespace Mir {
    class MIR_API Log{
        public:
            static void Init();
            inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_CoreLogger; }
            inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_ClientLogger; }
            Log();
            ~Log();
        private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger; 

    };
}

// Core Log Macros
#define MIR_CORE_TRACE(...) ::Mir::Log::getCoreLogger()->trace(__VA_ARGS__)
#define MIR_CORE_INFO(...) ::Mir::Log::getCoreLogger()->info(__VA_ARGS__)
#define MIR_CORE_WARN(...) ::Mir::Log::getCoreLogger()->warn(__VA_ARGS__)
#define MIR_CORE_ERROR(...) ::Mir::Log::getCoreLogger()->error(__VA_ARGS__)
#define MIR_CORE_FATAL(...) ::Mir::Log::getCoreLogger()->fatal(__VA_ARGS__)


// Client Log Macros
#define MIR_CLIENT_TRACE(...) ::Mir::Log::getClientLogger()->trace(__VA_ARGS__)
#define MIR_CLIENT_INFO(...) ::Mir::Log::getClientLogger()->info(__VA_ARGS__)
#define MIR_CLIENT_WARN(...) ::Mir::Log::getClientLogger()->warn(__VA_ARGS__)
#define MIR_CLIENT_ERROR(...) ::Mir::Log::getClientLogger()->error(__VA_ARGS__)
#define MIR_CLIENT_FATAL(...) ::Mir::Log::getClientLogger()->fatal(__VA_ARGS__)
