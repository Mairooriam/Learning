#include "Mirpch.h"

#include "Application.h"
#include "Mir/Events/ApplicationEvent.h"



#include "Renderer/Renderer.h"
#include "Mir/Input.h"
#include <GLFW/glfw3.h>

#include "OpcUA/OpcuaClient.h"

#include "CPython/PythonTool.h"

namespace Mir {  

  #define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    Application::Application() 
        {
		MIR_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
        
        m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

        // PythonTool pythonTool;
    
        // // Path to your Python script
        // std::string scriptPath = "C:\\Users\\35850\\Desktop\\repositories\\learning2\\Learning\\Mir\\src\\Mir\\CPython\\excelTools.py";
        

        // std::vector<std::string> args = {
        //     "C:\\Users\\35850\\Desktop\\repositories\\learning2\\Learning\\Mir\\External\\testdata\\manual_edited.xlsx",
        //     "C:\\Users\\35850\\Desktop\\repositories\\learning2\\Learning\\Mir\\External\\testdata\\testdata222.csv"  // optional
        // };
        

        // pythonTool.RunPythonScript(scriptPath, args);    

        m_brParser = new brParser(); 
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

            float time = (float)glfwGetTime(); // TODO: abstract into Platfrom::GetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            for (Layer* layer : m_LayerStack){
                layer->OnUpdate(timestep);
            }
            
            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack){
                layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();

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

    void Application::CreateOpcUaClient(std::string_view serverAdress){
        OpcuaClient opcUaClient(serverAdress);
    }

    bool Application::OnwWindowClose(WindowCloseEvent &e)
    {
        (void)e; // Mark parameter as unused
        m_Running = false;
        return true;
    }
}