#include "mirpch.h"

#include "Application.h"
#include "Mir/Events/ApplicationEvent.h"
#include "Mir/log.h"

namespace Mir {

  #define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

    Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
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
    }
    void Application::PushOverlay(Layer *layer){
        m_LayerStack.PushOverlay(layer);
    }

    bool Application::OnwWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
}
