#pragma once

#include "Mir/core.h"
#include "Events/Event.h"
#include "Window.h"
namespace Mir {
    class MIR_API Application {
    public:
        Application();
        virtual ~Application();
        void Run();
    private:
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

    Application* createApplication();
}