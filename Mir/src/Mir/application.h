#pragma once

#include "Core.h"

#include "Window.h"
#include "Mir/LayerStack.h"
#include "Mir/Events/ApplicationEvent.h"
#include "Mir/Events/Event.h"

#include "Mir/ImGui/ImGuiLayer.h"

#include "Mir/Core/Timestep.h"

#include "b&rParser/parser.h"

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

        inline brParser& GetBrParser() { return *m_brParser; }

        void CreateOpcUaClient(std::string_view serverAdress);
        
    private:
        bool OnwWindowClose(WindowCloseEvent& e);
    private:
        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;
        brParser* m_brParser;
    private:
        static Application* s_Instance; 
    };

    Application* createApplication();
}