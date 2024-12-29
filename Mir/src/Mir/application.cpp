#include "mirpch.h"

#include "Application.h"
#include "Mir/Events/ApplicationEvent.h"
#include "Mir/log.h"

namespace Mir {
    Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
    }


    Application::~Application() {
    }

    void Application::Run()
    {

		while (m_Running){
			m_Window->OnUpdate();
		}
    }

}

