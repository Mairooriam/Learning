#pragma once

#include "Core.h"

#include "Window.h"
#include "Mir/LayerStack.h"
#include "Mir/Events/ApplicationEvent.h"
#include "Mir/Events/Event.h"


#include "Mir/ImGui/ImGuiLayer.h"

namespace Mir {
    class MIR_API Application {
    public:
        Application();
        virtual ~Application();
        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        inline Window& GetWindow() { return *m_Window;}
        
        inline static Application& Get() { return *s_Instance; }
    private:
        bool OnwWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        LayerStack m_LayerStack;
    private:
        static Application* s_Instance; 
    };

    Application* createApplication();
}