#pragma once

#include "Mir/core.h"
#include "Events/Event.h"
namespace Mir {
    class MIR_API Application {
    public:
        Application();
        virtual ~Application();
        void Run();
    };

    Application* createApplication();
}