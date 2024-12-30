#pragma once

#include "core.h"

#include "Window.h"
#include "Mir/LayerStack.h"
#include "Mir/Events/ApplicationEvent.h"
#include "Mir/Events/Event.h"

namespace Mir {
    class MIR_API Application {
    public:
        Application();
        virtual ~Application();
        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
    private:
        bool OnwWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;
    };

    Application* createApplication();
}