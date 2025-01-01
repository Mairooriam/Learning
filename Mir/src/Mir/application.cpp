#include "Mirpch.h"

#include "Application.h"
#include "Mir/Events/ApplicationEvent.h"

#include <glad/glad.h>
namespace Mir {

  #define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    Application::Application() {
        MIR_CORE_ASSERT(!s_Instance, "Application already exists!");
        
        s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
        unsigned int id;
        glGenVertexArrays(1, &id);
        MIR_CORE_INFO("id:{0}",id);
    }

    void Application::OnEvent(Event &e){        
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnwWindowClose));

        MIR_CORE_TRACE("{0}", e);
    }

    Application::~Application() {
    }

    void Application::Run()
    {

		while (m_Running){
            for (Layer* layer : m_LayerStack)
            {
                layer->OnUpdate();
            }
            

			m_Window->OnUpdate();
		}
    }

   

    void Application::PushLayer(Layer *layer){
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }
    void Application::PushOverlay(Layer *layer){
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    bool Application::OnwWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
}
