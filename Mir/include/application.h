#pragma once

#include "core.h"

namespace Mir {
    class MIR_API Application {
    public:
        Application();
        virtual ~Application();
        void Run();
    };
}