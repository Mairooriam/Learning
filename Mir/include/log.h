#pragma once

#include "core.h"
#include <spdlog/spdlog.h>

namespace Mir {
    class MIR_API Log{
        public:
            static void init();
            inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_CoreLogger; }
            inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_ClientLogger; }
            Log();
            ~Log();
        private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger; 

    };
}