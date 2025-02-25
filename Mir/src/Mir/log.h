#pragma once

#include "Core.h"
#include "Mirpch.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>

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
// syntax example MIR_CORE_WARN("Windows file explorer: ENCOUNTERED AN ERROR: {0}", errormsg);
#define MIR_CORE_TRACE(...) ::Mir::Log::getCoreLogger()->trace(__VA_ARGS__)
#define MIR_CORE_INFO(...) ::Mir::Log::getCoreLogger()->info(__VA_ARGS__)
#define MIR_CORE_WARN(...) ::Mir::Log::getCoreLogger()->warn(__VA_ARGS__)
#define MIR_CORE_ERROR(...) ::Mir::Log::getCoreLogger()->error(__VA_ARGS__)
#define MIR_CORE_FATAL(...) ::Mir::Log::getCoreLogger()->fatal(__VA_ARGS__)


// Client Log Macros
#define MIR_TRACE(...) ::Mir::Log::getClientLogger()->trace(__VA_ARGS__)
#define MIR_INFO(...) ::Mir::Log::getClientLogger()->info(__VA_ARGS__)
#define MIR_WARN(...) ::Mir::Log::getClientLogger()->warn(__VA_ARGS__)
#define MIR_ERROR(...) ::Mir::Log::getClientLogger()->error(__VA_ARGS__)
#define MIR_FATAL(...) ::Mir::Log::getClientLogger()->fatal(__VA_ARGS__)
