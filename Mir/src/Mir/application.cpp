#include "mirpch.h"

#include "Application.h"
#include "Mir/Events/ApplicationEvent.h"
#include "Mir/log.h"

namespace Mir {
    Application::Application() {
    }


    Application::~Application() {
    }

    void Application::Run()
    {
        WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			MIR_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			MIR_TRACE(e);
		}
		while (true);
    }

}

