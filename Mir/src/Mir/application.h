#pragma once

#include "Core.h"

#include "Window.h"
#include "Mir/LayerStack.h"
#include "Mir/Events/ApplicationEvent.h"
#include "Mir/Events/Event.h"

#include "Mir/ImGui/ImGuiLayer.h"

#include "Mir/Renderer/Shader.h"

#include "Mir/Renderer/Buffer.h"

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
        inline static VertexBuffer& GetVertexBuffer() { return *s_Instance->m_VertexBuffer; }
    private:
        bool OnwWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        LayerStack m_LayerStack;

        unsigned int m_VertexArray;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<Shader> m_Shader;
    private:
        static Application* s_Instance; 
    };

    Application* createApplication();
}