#include "Mirpch.h"

#include "Application.h"
#include "Mir/Events/ApplicationEvent.h"


#include <glad/glad.h>

#include "Mir/Input.h"

#include <glm/glm.hpp>
namespace Mir {

  #define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    Application::Application() {
        MIR_CORE_ASSERT(!s_Instance, "Application already exists!");
        
        s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
    }

    void Application::OnEvent(Event &e){        
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnwWindowClose));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
    }

    Application::~Application() {
    }

    void Application::Run()
    {


		while (m_Running){

            glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

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
